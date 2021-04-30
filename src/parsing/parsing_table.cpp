#include "parsing/parsing_table.h"

namespace jucc::parsing {

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

void ParsingTable::SetFirsts(utils::SymbolsMap firsts) { firsts_ = std::move(firsts); }

void ParsingTable::SetProductions(grammar::Productions productions) { productions_ = std::move(productions); }

void ParsingTable::SetFollows(utils::SymbolsMap follows) { follows_ = std::move(follows); }
}  // namespace jucc::parsing
