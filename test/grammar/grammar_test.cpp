#include "grammar/grammar.h"

#include "gtest/gtest.h"

using jucc::grammar::Parser;

TEST(grammar, parserTest0) {
  Parser parser = Parser("../test/grammar/grammar_test_0.g");
  ASSERT_EQ(true, parser.Parse());
}

TEST(grammar, parserTest1) {
  Parser parser = Parser("../test/grammar/grammar_test_1.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %terminals", parser.GetError());
}

TEST(grammar, parserTest2) {
  Parser parser = Parser("../test/grammar/grammar_test_2.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %non_terminals", parser.GetError());
}

TEST(grammar, parserTest3) {
  Parser parser = Parser("../test/grammar/grammar_test_3.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %rules", parser.GetError());
}

TEST(grammar, parserTest4) {
  Parser parser = Parser("../test/grammar/grammar_test_4.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %", parser.GetError());
}

TEST(grammar, parserTest5) {
  Parser parser = Parser("../test/grammar/grammar_test_5.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token outside block: bruh", parser.GetError());
}

TEST(grammar, parserTest6) {
  Parser parser = Parser("../test/grammar/grammar_test_6.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parserTest7) {
  Parser parser = Parser("../test/grammar/grammar_test_7.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parserTestFile) {
  Parser parser = Parser("invalid_file_path");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: file not found", parser.GetError());
}
