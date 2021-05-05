#ifndef JUCC_PARSER_PARSER_H
#define JUCC_PARSER_PARSER_H

#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "parser/parsing_table.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parser {

class Parser {
  /**
   * A stack to put the symbols and perform the actual parsing
   */
  std::stack<std::string> stack_;

  /**
   * The given input string to parse.
   */
  std::vector<std::string> input_string_;

  /**
   * The start symbol for the grammar
   */
  std::string start_symbol_;

  /**
   * Holds the current step of parsing.
   */
  int current_step_{0};

  /**
   * Holds the build up parsing table object
   */
  ParsingTable table_;

  /**
   * Holds the history of the productions parsed during parsing
   */
  std::vector<int> production_history_;

  /**
   * Holds a copy of the input string initially
   * and changes with each step of parsing.
   */
  std::vector<std::string> current_string_;

  /**
   * Errors incurred, if any, during the construction of the
   * LL(1) parsing table.
   */
  std::vector<std::string> parsing_table_errors_;

  /**
   * Errors incurred during the parsing of the given input file.
   */
  std::vector<std::string> parser_errors_;

 public:
  /**
   * Constructor for initializing stack and other members.
   */
  Parser();

  /**
   * Used for parsing the next token of the input string
   */
  void ParseNextStep();

  /**
   * Resets the entire parsing process
   */
  void ResetParsing();

  /**
   * Function that returns true when the parsing is completed
   */
  bool IsComplete();

  /**
   * Completes a step of parsing
   */
  void DoNextStep();

  /* getters and setters*/
  void SetInputString(std::vector<std::string> inps);
  void SetParsingTable(ParsingTable table);
  void SetStartSymbol(std::string start);
  void SetParsingTableErrors(std::vector<std::string> errs) { parsing_table_errors_ = std::move(errs); }
  [[nodiscard]] const std::vector<int> &GetProductionHistory() { return production_history_; }
  [[nodiscard]] const std::vector<std::string> &GetParsingTableErrors() { return parsing_table_errors_; }
  [[nodiscard]] const std::vector<std::string> &GetParserErrors() { return parser_errors_; }
};
}  // namespace parser

}  // namespace jucc

#endif
