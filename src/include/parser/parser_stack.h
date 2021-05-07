#ifndef JUCC_PARSER_PARSER_STACK_H
#define JUCC_PARSER_PARSER_STACK_H

#include <stack>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace jucc::parser {
class ParserStack : public std::stack<std::string> {
  /**
   * pretty print parse_tree_ indentation spaces
   */
  static const int INDENTATION = 4;

  /**
   * Stores the actual parse tree for Treant.js integration
   */
  json parse_tree_;

  /**
   * required to track traversal order
   */
  std::stack<json *> parent_;

  /**
   * required to handle duplicate symbol naming
   */
  int symbol_counter_{0};

  /**
   * utility function to update parse_tree_ and parent_ on push
   */
  void HandlePush(const std::string & /*value*/);

 public:
  /**
   * Constructor to call base class constructor and initialize class members
   */
  ParserStack() : std::stack<std::string>(), parse_tree_(json::object({})) { parent_.push(&parse_tree_); }

  /**
   * @override the base class stack pop method.
   * Track stack pops in stack to adjust parse_tree_
   */
  void pop();

  /**
   * @override the base class stack push methods.
   * Track stack pushes in stack to adjust parse_tree_
   */
  void push(const std::string & /*value*/);
  void push(std::string && /*value*/);

  /**
   * It's time to pretty print!!!
   * @returns the parse_tree_ as a formatted string
   */
  std::string GetFormattedJson() { return parse_tree_.dump(INDENTATION); }

  /* getter for parse_tree_ */
  [[nodiscard]] const json &GetParseTree() { return parse_tree_; }
};
}  // namespace jucc::parser

#endif
