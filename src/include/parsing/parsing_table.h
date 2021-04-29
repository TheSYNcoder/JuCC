#ifndef JUCC_PARSING_PARSING_TABLE_H
#define JUCC_PARSING_PARSING_TABLE_H

#include <string>
#include <unordered_map>

#include "grammar/grammar.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parsing {

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

 public:
  /**
   * Used for setting synchronization tokens in the parsing table calculated from the
   * follow set.
   */
  const std::string SYNCH_TOKEN{"synch"};

  ParsingTable() = default;

  /**
   * Builds the parsing table from the firsts and follows
   */
  void BuildTable();

  /**
   * Gets the entry in the parsing table corresponding to a terminal and a non-terminal
   */
  void GetEntry(std::string non_terminal_, std::string terminal_);

  /**
   * Setter for the first set.
   */
  void SetFirsts();

  /**
   * Setter for the follow set.
   */
  void SetFollows();
};

}  // namespace parsing
}  // namespace jucc

#endif
