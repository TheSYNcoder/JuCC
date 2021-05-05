#include "parser/parsing_table.h"

#include <algorithm>
#include <sstream>

namespace jucc::parser {

void ParsingTable::BuildTable() {
  // fill initially all errors
  for (auto &nt : non_terminals_) {
    for (auto &t : terminals_) {
      table_[nt][t] = std::string(ERROR_TOKEN);
    }
  }

  // We consider that the symbols on the Follow(A) to be in the synchronization set
  for (auto &nt : non_terminals_) {
    if (follows_.count(nt) != 0U) {
      for (const auto &symbol : follows_[nt]) {
        if (table_[nt][symbol] != std::string(ERROR_TOKEN)) {
          std::string ret;
          ret += "Error duplicate entry in parsing table, ";
          ret += "Production : " + nt;
          ret += " Symbol : " + symbol;
          errors_.push_back(ret);
        }
        table_[nt][symbol] = std::string(SYNCH_TOKEN);
      }
    }
  }

  // firsts
  for (int prod_no = 0; prod_no < static_cast<int>(productions_.size()); prod_no++) {
    auto rules = productions_[prod_no].GetRules();
    for (int rule_no = 0; rule_no < static_cast<int>(rules.size()); rule_no++) {
      std::string first_entity = rules[rule_no].GetEntities()[0];

      // check if first_entity is terminal
      if (std::find(terminals_.begin(), terminals_.end(), first_entity) != terminals_.end()) {
        std::string entry = table_[productions_[prod_no].GetParent()][first_entity];
        if ((entry != std::string(ERROR_TOKEN)) && (entry != std::string(SYNCH_TOKEN))) {
          std::string ret;
          ret += "Error duplicate entry in parsing table, ";
          ret += "Production : " + productions_[prod_no].GetParent();
          ret += " Symbol : " + first_entity;
          errors_.push_back(ret);
        }
        table_[productions_[prod_no].GetParent()][first_entity] = std::to_string(prod_no * 100 + rule_no);
      } else if (std::find(non_terminals_.begin(), non_terminals_.end(), first_entity) != non_terminals_.end()) {
        // first entity is a non-terminal
        if (firsts_.count(first_entity) != 0U) {
          for (auto &symbol : firsts_[first_entity]) {
            if (symbol != std::string(grammar::EPSILON)) {
              std::string entry = table_[productions_[prod_no].GetParent()][symbol];
              if ((entry != std::string(ERROR_TOKEN)) && (entry != std::string(SYNCH_TOKEN))) {
                std::string ret;
                ret += "Error duplicate entry in parsing table, ";
                ret += "Production : ";
                ret = ret.append(productions_[prod_no].GetParent());
                ret += " Symbol : ";
                ret = ret.append(symbol);
                errors_.push_back(ret);
              }
              table_[productions_[prod_no].GetParent()][symbol] = std::to_string(prod_no * 100 + rule_no);
            }
          }
        }

      } else if (first_entity == std::string(grammar::EPSILON)) {
        // first entity is epsilon
        if (follows_.count(productions_[prod_no].GetParent()) != 0U) {
          for (auto &symbol : follows_[productions_[prod_no].GetParent()]) {
            std::string entry = table_[productions_[prod_no].GetParent()][symbol];
            if ((entry != std::string(ERROR_TOKEN)) && (entry != std::string(SYNCH_TOKEN))) {
              std::string ret;
              ret += "Error duplicate entry in parsing table, ";
              ret += "Production : " + productions_[prod_no].GetParent();
              ret += " Symbol : " + symbol;
              errors_.push_back(ret);
            }
            table_[productions_[prod_no].GetParent()][symbol] = std::to_string(prod_no * 100 + rule_no);
          }
        }
      }
    }
  }
}

std::pair<int, int> ParsingTable::GetEntry(const std::string &non_terminal_, const std::string &terminal_) {
  std::string entry = table_[non_terminal_][terminal_];
  int value;
  std::stringstream ss(entry);
  ss >> value;
  return std::make_pair(value / 100, value % 100);
}
}  // namespace jucc::parser
