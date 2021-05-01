#ifndef JUCC_PARSING_PARSING_H
#define JUCC_PARSING_PARSING_H

#include <algorithm>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "parsing/utils/parsing_table.h"
#include "utils/first_follow.h"

namespace jucc {

namespace parsing {

class Parsing {
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
  parsing_table::ParsingTable table_;

  /**
   * Holds the history of the productions parsed during parsing
   */
  std::vector<int> production_history_;

  /**
   * Holds a copy of the input string initially
   * and changes with each step of parsing.
   */
  std::vector<std::string> current_string_;

 public:
  /**
   * Default constructor
   */
  Parsing() = default;

  /**
   * Initialization of the stack and other members
   */
  void Init();

  /**
   * Used for parsing the next token of the input string
   */
  void ParseNextStep();

  /**
   * Resets the entire parsing process
   */
  void ResetParsing();

  /**
   * Setter for the input string
   */
  void SetInputString(std::vector<std::string> inps);

  /**
   * Setter for parsing table
   */
  void SetParsingTable(parsing_table::ParsingTable table);

  /**
   * Setter for the start symbol
   */
  void SetStartSymbol(std::string start);

  /**
   * Function that returns true when the parsing is completed
   */
  bool IsComplete();

  /**
   * Completes a step of parsing
   */
  void DoNextStep();

  /**
   * Getter for production history
   */
  [[nodiscard]] const std::vector<int> &GetProductionHistory();
};
}  // namespace parsing

}  // namespace jucc

#endif
