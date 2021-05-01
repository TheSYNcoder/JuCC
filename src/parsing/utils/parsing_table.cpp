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
      if (find(terminals_.begin(), terminals_.end(), first_entity) != terminals_.end()) {
        table_[productions_[prod_no].GetParent()][first_entity] = std::to_string(prod_no * 100 + rule_no);
      } else if (find(non_terminals_.begin(), non_terminals_.end(), first_entity) != non_terminals_.end()) {
        // first entity is a non-terminal
        if (firsts_.count(first_entity) != 0U) {
          for (auto &symbol : firsts_[first_entity]) {
            if (symbol != std::string(grammar::EPSILON)) {
              table_[productions_[prod_no].GetParent()][symbol] = std::to_string(prod_no * 100 + rule_no);
            }
          }
          // if epsilon present add production under follow(A) symbols
          if (find(firsts_[first_entity].begin(), firsts_[first_entity].end(), std::string(grammar::EPSILON)) !=
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

void ParsingTable::PrettyPrintFirsts() {
  std::cout << "\nFIRSTS\n\n";
  std::cout << "SYMBOL\tFIRST_SET\n";
  for (auto &entry : firsts_) {
    std::cout << entry.first << "\t";
    std::cout << "{ ";
    for (auto &v : entry.second) {
      std::cout << v << " , ";
    }
    std::cout << " }\n";
  }
}

void ParsingTable::PrettyPrintFollows() {
  std::cout << "\nFOLLOWS\n\n";

  std::cout << "SYMBOL\tFOLLOW_SET\n";
  for (auto &entry : follows_) {
    std::cout << entry.first << "\t";
    std::cout << "{ ";
    for (auto &v : entry.second) {
      std::cout << v << " , ";
    }
    std::cout << " }\n";
  }
}

void ParsingTable::PrettyPrintTable() {
  std::cout << "\nLL(1) PARSING TABLE\n\n";
  for (int i = 0; i <= non_terminals_.size(); i++) {
    for (int j = 0; j <= terminals_.size(); j++) {
      if ((i == 0) && (j == 0)) {
        std::cout << "\t";
      } else if (i == 0) {
        std::cout << terminals_[j - 1] << "\t";
      } else if (j == 0) {
        std::cout << non_terminals_[i - 1] << "\t";
      } else {
        std::cout << table_[non_terminals_[i - 1]][terminals_[j - 1]] << "\t";
      }
      if (j == terminals_.size()) {
        std::cout << "\n";
      }
    }
  }
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
