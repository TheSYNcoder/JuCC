#include "parser/parser.h"

#include <algorithm>
#include <fstream>

namespace jucc::parser {

Parser::Parser() {
  // initialize the stack
  stack_.push(std::string(utils::STRING_ENDMARKER));
  input_string_.clear();
  current_string_.clear();
}

std::string Parser::GenerateErrorMessage(const std::string &current_token) {
  std::string ret_string;
  ret_string += "parser error: at symbol: " + current_token;
  return ret_string;
}

void Parser::SetInputString(std::vector<std::string> inps) {
  if (!inps.empty()) {
    input_string_ = std::move(inps);
    current_string_ = input_string_;
    // augmented string for parsing
    current_string_.emplace_back(std::string(utils::STRING_ENDMARKER));
  }
}

void Parser::SetStartSymbol(std::string start) {
  start_symbol_ = std::move(start);
  stack_.push(start_symbol_);
}

void Parser::SetParsingTable(ParsingTable table) { table_ = std::move(table); }

bool Parser::IsComplete() {
  return (current_step_ == static_cast<int>(current_string_.size())) ||
         stack_.top() == std::string(utils::STRING_ENDMARKER);
}

void Parser::ResetParsing() {
  while (!stack_.empty()) {
    stack_.pop();
  }
  stack_.push(std::string(utils::STRING_ENDMARKER));
  stack_.push(start_symbol_);
  current_string_ = input_string_;
  current_step_ = 0;
  current_string_.emplace_back(std::string(utils::STRING_ENDMARKER));
}

void Parser::DoNextStep() {
  if (!IsComplete()) {
    current_step_++;
  }
}

void Parser::ParseNextStep() {
  std::string top_symbol = stack_.top();
  std::string current_token = current_string_[current_step_];
  ParsingTable::Table table = table_.GetTable();
  // skip tokens until it is in the first or is a synch token
  while (!IsComplete() && table[top_symbol][current_token] == std::string(ERROR_TOKEN)) {
    parser_errors_.push_back(GenerateErrorMessage(current_token));
    DoNextStep();
    if (current_step_ < static_cast<int>(current_string_.size())) {
      current_token = current_string_[current_step_];
    }
  }
  if (!IsComplete()) {
    // if SYNCH TOKEN - We skip the current symbol on stack top
    if (table[top_symbol][current_token] == std::string(SYNCH_TOKEN)) {
      parser_errors_.push_back(GenerateErrorMessage(current_token));
      stack_.pop();
    } else {
      auto terminals = table_.GetTerminals();
      // check if current stack top matches the current token
      if (top_symbol == current_token) {
        stack_.pop();
        DoNextStep();
      } else if (std::find(terminals.begin(), terminals.end(), top_symbol) != terminals.end() &&
                 std::find(terminals.begin(), terminals.end(), current_token) != terminals.end()) {
        parser_errors_.push_back(GenerateErrorMessage(current_token));
        DoNextStep();
      } else {
        // we expand the production
        auto prod_rule = table_.GetEntry(top_symbol, current_token);
        auto productions = table_.GetProductions();
        auto req_rule = productions[prod_rule.first].GetRules()[prod_rule.second];
        auto entities = req_rule.GetEntities();
        std::reverse(entities.begin(), entities.end());
        stack_.pop();
        if (!entities.empty() && entities[0] == std::string(grammar::EPSILON)) {
          production_history_.push_back(prod_rule.first * 100 + prod_rule.second);
          return;
        }
        for (auto &entity : entities) {
          stack_.push(entity);
        }
        production_history_.push_back(prod_rule.first * 100 + prod_rule.second);
      }
    }
  }
}

void Parser::BuildParseTree() {
  parse_tree_ = json::object({});

  // if errors cannot build tree
  if (!parser_errors_.empty()) {
    return;
  }

  // init parse tree state
  parse_tree_[start_symbol_] = json::object({});
  std::stack<json *> parent_node_stack;
  parent_node_stack.push(&parse_tree_[start_symbol_]);

  auto productions = table_.GetProductions();
  auto terminals = table_.GetTerminals();
  auto non_terminals = table_.GetNonTerminals();
  // iterate over production history and build tree
  for (const auto &prod : production_history_) {
    int production_index = prod / 100;
    int rule_index = prod % 100;
    auto parent = productions[production_index].GetParent();
    auto rule = productions[production_index].GetRules()[rule_index];

    auto entities = rule.GetEntities();
    // augment_entities to handle duplicates
    std::unordered_map<std::string, int> symbol_count;
    std::unordered_map<std::string, std::string> default_name;
    for (auto &entity : entities) {
      auto p_entity = entity;
      if (symbol_count[entity]++ != 0) {
        entity += "_" + std::to_string(symbol_count[entity] - 1);
      }
      default_name[entity] = p_entity;
    }

    json *parent_node = parent_node_stack.top();
    // add entities to current parent node
    for (const auto &entity : entities) {
      if (std::find(terminals.begin(), terminals.end(), default_name[entity]) != terminals.end()) {
        (*parent_node)[entity] = json();
      } else {
        (*parent_node)[entity] = json::object({});
      }
    }

    // update parent_node_stack
    parent_node_stack.pop();
    std::reverse(entities.begin(), entities.end());
    for (const auto &entity : entities) {
      if (std::find(non_terminals.begin(), non_terminals.end(), default_name[entity]) != non_terminals.end()) {
        parent_node_stack.push(&((*parent_node)[entity]));
      }
    }
  }
}

bool Parser::WriteParseTree(const std::string &filepath) {
  std::ofstream ofs(filepath);
  if (ofs.is_open()) {
    ofs << parse_tree_.dump(INDENTATION) << '\n';
    return true;
  }

  return false;
}

}  // namespace jucc::parser
