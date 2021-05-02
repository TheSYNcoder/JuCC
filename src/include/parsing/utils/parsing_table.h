#ifndef JUCC_PARSING_UTILS_PARSING_TABLE_H
#define JUCC_PARSING_UTILS_PARSING_TABLE_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "grammar/grammar.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parsing_table {

const char SYNCH_TOKEN[] = "synch";

const char ERROR_TOKEN[] = "error";

class ParsingTable {
 public:
  using Table = std::unordered_map<std::string, std::unordered_map<std::string, std::string> >;

 private:
  /**
   * stores the parsing table, which is calculated from the productions in the grammar
   * and it's first and follow set
   */
  Table table_;

  /**
   * Stores firsts of the productions
   */
  utils::SymbolsMap firsts_;

  /**
   * Stores the follows of the productions
   */
  utils::SymbolsMap follows_;

  /**
   * Stores the productions of the grammar.
   */
  grammar::Productions productions_;

  /**
   * Stores the terminals of the grammar
   */
  std::vector<std::string> terminals_;

  /**
   * Stores the non-termninals of the grammar.
   */
  std::vector<std::string> non_terminals_;

 public:
  /**
   * Default constructor
   */
  ParsingTable() = default;

  /**
   * Used for setting synchronization tokens in the parsing table calculated from the
   * follow set.
   */
  ParsingTable(std::vector<std::string> terms, std::vector<std::string> non_terms)
      : terminals_(std::move(terms)), non_terminals_(std::move(non_terms)) {}

  /**
   * Builds the parsing table from the firsts and follows
   */
  void BuildTable();

  /**
   * Gets the entry in the parsing table corresponding to a terminal and a non-terminal
   * Gets the production and the rule number
   */
  std::pair<int, int> GetEntry(const std::string &non_terminal_, const std::string &terminal_);

  /**
   * Setter for the first set.
   */
  void SetFirsts(utils::SymbolsMap firsts);

  /**
   * Pretty print firsts set
   */
  std::string PrettyPrintFirsts();

  /**
   * Pretty print follows set
   */
  std::string PrettyPrintFollows();

  /**
   * Pretty print follows set
   */
  std::string PrettyPrintTable();

  /**
   * Setter for the first set.
   */
  void SetProductions(grammar::Productions productions);

  /**
   * Setter for the follow set.
   */
  void SetFollows(utils::SymbolsMap follows);

  /**
   * Getter for the firsts set
   */
  [[nodiscard]] const utils::SymbolsMap &GetFirsts();

  /**
   * Getter for the follows set
   */
  [[nodiscard]] const utils::SymbolsMap &GetFollows();

  /**
   * Getter for the productions
   */
  [[nodiscard]] const grammar::Productions &GetProductions();

  /**
   * Getter for the non terminals
   */
  [[nodiscard]] const std::vector<std::string> &GetNonTerminals();

  /**
   * Getter for the terminals
   */
  [[nodiscard]] const std::vector<std::string> &GetTerminals();

  /**
   * Getter for the parsing table
   */
  [[nodiscard]] const Table &GetTable();
};

}  // namespace parsing_table
}  // namespace jucc

#endif
