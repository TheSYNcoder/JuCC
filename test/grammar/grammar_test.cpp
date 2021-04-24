#include "grammar/grammar.h"

#include "gtest/gtest.h"

using jucc::grammar::Parser;

TEST(grammar, parser_test_0) {
  Parser parser = Parser("../test/grammar/grammar_test_0.g");
  ASSERT_EQ(true, parser.Parse());
}

TEST(grammar, parser_test_1) {
  Parser parser = Parser("../test/grammar/grammar_test_1.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %terminals", parser.GetError());
}

TEST(grammar, parser_test_2) {
  Parser parser = Parser("../test/grammar/grammar_test_2.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %non_terminals", parser.GetError());
}

TEST(grammar, parser_test_3) {
  Parser parser = Parser("../test/grammar/grammar_test_3.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %rules", parser.GetError());
}

TEST(grammar, parser_test_4) {
  Parser parser = Parser("../test/grammar/grammar_test_4.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %", parser.GetError());
}

TEST(grammar, parser_test_5) {
  Parser parser = Parser("../test/grammar/grammar_test_5.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token outside block: bruh", parser.GetError());
}

TEST(grammar, parser_test_6) {
  Parser parser = Parser("../test/grammar/grammar_test_6.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parser_test_7) {
  Parser parser = Parser("../test/grammar/grammar_test_7.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parser_test_na) {
  Parser parser = Parser("invalid_file_path");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: file not found", parser.GetError());
}
