#ifndef JUCC_PARSING_PARSING_TABLE_H
#define JUCC_PARSING_PARSING_TABLE_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "grammar/grammar.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parsing {

const char SYNCH_TOKEN[] = "synch";

const char ERROR_TOKEN[] = "error";

class ParsingTable {
  using Table = std::unordered_map<std::string, std::unordered_map<std::string, std::string> >;

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
   * Setter for the first set.
   */
  void SetProductions(grammar::Productions productions);

  /**
   * Setter for the follow set.
   */
  void SetFollows(utils::SymbolsMap follows);
};

}  // namespace parsing
}  // namespace jucc

#endif
