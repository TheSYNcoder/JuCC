#ifndef JUCC_PARSER_PARSER_H
#define JUCC_PARSER_PARSER_H

#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"
#include "parser/parsing_table.h"
#include "utils/first_follow.h"

using json = nlohmann::ordered_json;

namespace jucc {
namespace parser {

class Parser {
  /**
   *  json pretty print indentation for generated parse tree
   */
  static const int INDENTATION = 4;

  /**
   * parse tree for Treant.js integration
   */
  json parse_tree_;

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
   * Errors incurred during the parsing of the given input file.
   */
  std::vector<std::string> parser_errors_;

  /**
   * Helper function to generate error messages for parsing.
   */
  static std::string GenerateErrorMessage(const std::string &current_token);

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

  /**
   * Build the parse tree from production history
   * Parse tree not built if parser in error state
   */
  void BuildParseTree();

  /**
   * Dumps the parse tree in given path in json format
   * @returns true on success
   */
  bool WriteParseTree(const std::string &filepath);

  /* getters and setters*/
  void SetInputString(std::vector<std::string> inps);
  void SetParsingTable(ParsingTable table);
  void SetStartSymbol(std::string start);
  [[nodiscard]] const std::vector<int> &GetProductionHistory() { return production_history_; }
  [[nodiscard]] const std::vector<std::string> &GetParserErrors() { return parser_errors_; }
  [[nodiscard]] const json &GetParseTree() { return parse_tree_; }
};
}  // namespace parser

}  // namespace jucc

#endif
