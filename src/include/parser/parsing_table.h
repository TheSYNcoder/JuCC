#ifndef JUCC_PARSER_PARSING_TABLE_H
#define JUCC_PARSER_PARSING_TABLE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "grammar/grammar.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parser {

const char SYNCH_TOKEN[] = "synch";
const char ERROR_TOKEN[] = "error";

class ParsingTable {
 public:
  using Table = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

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

  /**
   * Stores the errors if any, in case of a non - LL(1)
   * grammar during the construction of the parsing table.
   */
  std::vector<std::string> errors_;

  /**
   * A helper function to generate error message.
   */
  std::string GenerateMessage(std::string production, std::string symbol);

 public:
  /**
   * Default constructor
   */
  ParsingTable() = default;

  /**
   * Used for setting synchronization tokens in the parsing table calculated from the
   * follow set. Adds "$" to terminals_.
   */
  ParsingTable(std::vector<std::string> terms, std::vector<std::string> non_terms)
      : terminals_(std::move(terms)), non_terminals_(std::move(non_terms)) {
    terminals_.emplace_back(utils::STRING_ENDMARKER);
  }

  /**
   * Builds the parsing table from the firsts and follows
   */
  void BuildTable();

  /**
   * Gets the entry in the parsing table corresponding to a terminal and a non-terminal
   * Gets the production and the rule number
   */
  std::pair<int, int> GetEntry(const std::string &non_terminal_, const std::string &terminal_);

  /* getters and setters */
  void SetFirsts(utils::SymbolsMap firsts) { firsts_ = std::move(firsts); }
  void SetProductions(grammar::Productions productions) { productions_ = std::move(productions); }
  void SetFollows(utils::SymbolsMap follows) { follows_ = std::move(follows); }
  [[nodiscard]] const utils::SymbolsMap &GetFirsts() { return firsts_; }
  [[nodiscard]] const utils::SymbolsMap &GetFollows() { return follows_; }
  [[nodiscard]] const grammar::Productions &GetProductions() { return productions_; }
  [[nodiscard]] const std::vector<std::string> &GetNonTerminals() { return non_terminals_; }
  [[nodiscard]] const std::vector<std::string> &GetTerminals() { return terminals_; }
  [[nodiscard]] const Table &GetTable() { return table_; }
  [[nodiscard]] const std::vector<std::string> &GetErrors() { return errors_; }
};

}  // namespace parser
}  // namespace jucc

#endif
