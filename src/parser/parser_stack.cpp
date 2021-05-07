#include "parser/parser_stack.h"

namespace jucc::parser {
void ParserStack::pop() {
  std::stack<std::string>::pop();
  parent_.pop();
}

void ParserStack::HandlePush(const std::string &value) {
  std::string augmented_value = value + "_" + std::to_string(symbol_counter_++);
  (*parent_.top())[augmented_value] = json::object({});
  parent_.push(&(*parent_.top())[augmented_value]);
}

void ParserStack::push(const std::string &value) {
  std::stack<std::string>::push(value);
  HandlePush(value);
}
void ParserStack::push(std::string &&value) {
  std::stack<std::string>::push(value);
  HandlePush(value);
}

}  // namespace jucc::parser
