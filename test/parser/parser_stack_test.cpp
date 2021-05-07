#include "parser/parser_stack.h"

#include <iostream>

#include "gtest/gtest.h"

using jucc::parser::ParserStack;

TEST(parser, ParserStack1) {
  ParserStack stack;
  stack.push("A");
  stack.push("A");
  stack.pop();
  stack.push("B");
  stack.push("A");
  stack.pop();
  stack.pop();
  stack.push("C");
  stack.pop();
  stack.pop();
  ASSERT_EQ(stack.GetParseTree().dump(), "{\"A_0\":{\"A_1\":{},\"B_2\":{\"A_3\":{}},\"C_4\":{}}}");
}

TEST(parser, ParserStack2) {
  ParserStack stack;
  stack.push("A");
  stack.push("A");
  stack.pop();
  stack.push("A");
  stack.push("A");
  stack.pop();
  stack.pop();
  stack.push("A");
  stack.pop();
  stack.pop();
  ASSERT_EQ(stack.GetParseTree().dump(), "{\"A_0\":{\"A_1\":{},\"A_2\":{\"A_3\":{}},\"A_4\":{}}}");
}
