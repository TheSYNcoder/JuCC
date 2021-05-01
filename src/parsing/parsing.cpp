#include "parsing/parsing.h"

#include <iostream>
namespace jucc::parsing {

void Parsing::Init() {
  // initialize the stack
  stack_.push("$");
  input_string_.clear();
  current_string_.clear();
}

void Parsing::SetInputString(std::vector<std::string> inps) {
  if (!inps.empty()) {
    input_string_ = std::move(inps);
    current_string_ = input_string_;
    // augmented string for parsing
    current_string_.emplace_back("$");
  }
}

void Parsing::SetStartSymbol(std::string start) {
  start_symbol_ = std::move(start);
  stack_.push(start_symbol_);
}

void Parsing::SetParsingTable(parsing_table::ParsingTable table) { table_ = std::move(table); }

bool Parsing::IsComplete() {
  return (current_step_ == static_cast<int>(current_string_.size()) - 1) || stack_.top() == "$";
}

void Parsing::ResetParsing() {
  current_string_ = input_string_;
  current_step_ = 0;
  current_string_.emplace_back("$");
}

void Parsing::DoNextStep() {
  if (!IsComplete()) {
    current_step_++;
  }
}

void Parsing::ParseNextStep() {
  std::string top_symbol = stack_.top();
  std::string current_token = current_string_[current_step_];
  parsing_table::ParsingTable::Table table = table_.GetTable();
  // skip tokens until it is in the first or is a synch token
  while (table[top_symbol][current_token] == std::string(parsing_table::ERROR_TOKEN)) {
    DoNextStep();
    current_token = current_string_[current_step_];
  }
  if (!IsComplete()) {
    // if SYNCH TOKEN - We skip the current symbol on stack top
    if (table[top_symbol][current_token] == std::string(parsing_table::SYNCH_TOKEN)) {
      stack_.pop();
    } else {
      // check if current stack top matches the current token
      if (top_symbol == current_token) {
        stack_.pop();
        DoNextStep();
      } else {
        // we expand the production
        auto prod_rule = table_.GetEntry(top_symbol, current_token);
        auto productions = table_.GetProductions();
        auto req_rule = productions[prod_rule.first].GetRules()[prod_rule.second];
        auto entities = req_rule.GetEntities();
        std::reverse(entities.begin(), entities.end());
        for (auto &entity : entities) {
          stack_.push(entity);
        }
        production_history_.push_back(prod_rule.first * 100 + prod_rule.second);
      }
    }
  }
}

const std::vector<int> &Parsing::GetProductionHistory() { return production_history_; }

}  // namespace jucc::parsing
