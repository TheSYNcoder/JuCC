#include "parsing/utils/parsing_table.h"

namespace jucc::parsing_table {

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
        table_[productions_[prod_no].GetParent()][first_entity] = std::to_string(prod_no * 100 + rule_no);
      } else if (std::find(non_terminals_.begin(), non_terminals_.end(), first_entity) != non_terminals_.end()) {
        // first entity is a non-terminal
        if (firsts_.count(first_entity) != 0U) {
          for (auto &symbol : firsts_[first_entity]) {
            if (symbol != std::string(grammar::EPSILON)) {
              table_[productions_[prod_no].GetParent()][symbol] = std::to_string(prod_no * 100 + rule_no);
            }
          }
          // if epsilon present add production under follow(A) symbols
          if (std::find(firsts_[first_entity].begin(), firsts_[first_entity].end(), std::string(grammar::EPSILON)) !=
              firsts_[first_entity].end()) {
            if (follows_.count(productions_[prod_no].GetParent()) != 0U) {
              for (auto &symbol : follows_[productions_[prod_no].GetParent()]) {
                table_[productions_[prod_no].GetParent()][symbol] = std::to_string(prod_no * 100 + rule_no);
              }
            }
          }
        }
      } else if (first_entity == std::string(grammar::EPSILON)) {
        // first entity is epsilon
        if (follows_.count(productions_[prod_no].GetParent()) != 0U) {
          for (auto &symbol : follows_[productions_[prod_no].GetParent()]) {
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

std::string ParsingTable::PrettyPrintFirsts() {
  std::string ret_string;
  ret_string += "\nFIRSTS\n\n";
  ret_string += "SYMBOL\tFIRST_SET\n";
  for (auto &entry : firsts_) {
    ret_string += std::string(entry.first) + "\t";
    ret_string += "{ ";
    for (auto &v : entry.second) {
      ret_string += std::string(v) + " , ";
    }
    ret_string += " }\n";
  }
  return ret_string;
}

std::string ParsingTable::PrettyPrintFollows() {
  std::string ret_string;
  ret_string += "\nFOLLOWS\n\n";

  ret_string += "SYMBOL\tFOLLOW_SET\n";
  for (auto &entry : follows_) {
    ret_string += std::string(entry.first) + "\t";
    ret_string += "{ ";
    for (auto &v : entry.second) {
      ret_string += std::string(v) + " , ";
    }
    ret_string += " }\n";
  }
  return ret_string;
}

std::string ParsingTable::PrettyPrintTable() {
  std::string ret_string;
  ret_string += "\nLL(1) PARSING TABLE\n\n";
  for (int i = 0; i <= static_cast<int>(non_terminals_.size()); i++) {
    for (int j = 0; j <= static_cast<int>(terminals_.size()); j++) {
      if ((i == 0) && (j == 0)) {
        ret_string += "\t";
      } else if (i == 0) {
        ret_string += std::string(terminals_[j - 1]) + "\t";
      } else if (j == 0) {
        ret_string += std::string(non_terminals_[i - 1]) + "\t";
      } else {
        ret_string += std::string(table_[non_terminals_[i - 1]][terminals_[j - 1]]) + "\t";
      }
      if (j == static_cast<int>(terminals_.size())) {
        ret_string += "\n";
      }
    }
  }
  return ret_string;
}

void ParsingTable::SetFirsts(utils::SymbolsMap firsts) { firsts_ = std::move(firsts); }

void ParsingTable::SetProductions(grammar::Productions productions) { productions_ = std::move(productions); }

void ParsingTable::SetFollows(utils::SymbolsMap follows) { follows_ = std::move(follows); }

const utils::SymbolsMap &ParsingTable::GetFirsts() { return firsts_; }

const utils::SymbolsMap &ParsingTable::GetFollows() { return follows_; }

const grammar::Productions &ParsingTable::GetProductions() { return productions_; }

const std::vector<std::string> &ParsingTable::GetNonTerminals() { return non_terminals_; }

const std::vector<std::string> &ParsingTable::GetTerminals() { return terminals_; }

const ParsingTable::Table &ParsingTable::GetTable() { return table_; }
}  // namespace jucc::parsing_table
