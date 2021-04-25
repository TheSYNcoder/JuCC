#include "grammar/grammar.h"

#include "gtest/gtest.h"

using jucc::grammar::Parser;

TEST(grammar, parser0) {
  Parser parser = Parser("../test/grammar/grammar_test_0.g");
  ASSERT_EQ(true, parser.Parse());
  ASSERT_EQ("<primary_expression>", parser.GetStartSymbol());
  std::vector<std::string> terminals = {"else",
                                        "float",
                                        "if",
                                        "int",
                                        "void",
                                        "(",
                                        ")",
                                        "{",
                                        "}",
                                        "*",
                                        "+",
                                        "-",
                                        "/",
                                        "%",
                                        "<<",
                                        ">>",
                                        "<",
                                        ">",
                                        "<=",
                                        ">=",
                                        "=",
                                        "==",
                                        "!=",
                                        ";",
                                        "identifier",
                                        "integer_constant",
                                        "float_constant"};
  ASSERT_EQ(terminals, parser.GetTerminals());
  std::vector<std::string> non_terminals = {"<primary_expression>", "<constant>"};
  ASSERT_EQ(non_terminals, parser.GetNonTerminals());

  ASSERT_EQ(2, parser.GetProductions().size());
  ASSERT_EQ("<constant>", parser.GetProductions()[0].GetParent());
  ASSERT_EQ("<primary_expression>", parser.GetProductions()[1].GetParent());

  ASSERT_EQ(3, parser.GetProductions()[1].GetRules().size());
  std::vector<std::string> rule0 = {"identifier", "<constant>"};
  std::vector<std::string> rule1 = {"<constant>"};
  std::vector<std::string> rule2 = {"EPSILON"};
  ASSERT_EQ(rule0, parser.GetProductions()[1].GetRules()[0].GetEntities());
  ASSERT_EQ(rule1, parser.GetProductions()[1].GetRules()[1].GetEntities());
  ASSERT_EQ(rule2, parser.GetProductions()[1].GetRules()[2].GetEntities());

  ASSERT_EQ(1, parser.GetProductions()[0].GetRules().size());
  std::vector<std::string> rule3 = {"integer_constant"};
  ASSERT_EQ(rule3, parser.GetProductions()[0].GetRules()[0].GetEntities());
}

TEST(grammar, parser1) {
  Parser parser = Parser("../test/grammar/grammar_test_1.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %terminals", parser.GetError());
}

TEST(grammar, parser2) {
  Parser parser = Parser("../test/grammar/grammar_test_2.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %non_terminals", parser.GetError());
}

TEST(grammar, parser3) {
  Parser parser = Parser("../test/grammar/grammar_test_3.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %rules", parser.GetError());
}

TEST(grammar, parser3b) {
  Parser parser = Parser("../test/grammar/grammar_test_3b.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %start", parser.GetError());
}

TEST(grammar, parser4) {
  Parser parser = Parser("../test/grammar/grammar_test_4.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token %", parser.GetError());
}

TEST(grammar, parserTest5) {
  Parser parser = Parser("../test/grammar/grammar_test_5.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: invalid token outside block: bruh", parser.GetError());
}

TEST(grammar, parser6) {
  Parser parser = Parser("../test/grammar/grammar_test_6.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parser7) {
  Parser parser = Parser("../test/grammar/grammar_test_7.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: EPSILON is reserved", parser.GetError());
}

TEST(grammar, parser8) {
  Parser parser = Parser("../test/grammar/grammar_test_8.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: ambiguous start symbol", parser.GetError());
}

TEST(grammar, parser9) {
  Parser parser = Parser("../test/grammar/grammar_test_9.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: production cannot start with EPSILON", parser.GetError());
}

TEST(grammar, parser10) {
  Parser parser = Parser("../test/grammar/grammar_test_10.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: rules syntax error ':' expected: bruh", parser.GetError());
}

TEST(grammar, parser11) {
  Parser parser = Parser("../test/grammar/grammar_test_11.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: rules syntax error ':' expected", parser.GetError());
}

TEST(grammar, parser12) {
  Parser parser = Parser("../test/grammar/grammar_test_12.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: block is incomplete '%' expected", parser.GetError());
}

TEST(grammar, parser13) {
  Parser parser = Parser("../test/grammar/grammar_test_13.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: inconsistent or duplicate terminals", parser.GetError());
}

TEST(grammar, parser14) {
  Parser parser = Parser("../test/grammar/grammar_test_14.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: inconsistent or duplicate non_terminals", parser.GetError());
}

TEST(grammar, parser15) {
  Parser parser = Parser("../test/grammar/grammar_test_15.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: terminals and non_terminals not disjoint", parser.GetError());
}

TEST(grammar, parser16) {
  Parser parser = Parser("../test/grammar/grammar_test_16.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: non_terminal not found: <bruh>", parser.GetError());
}

TEST(grammar, parser17) {
  Parser parser = Parser("../test/grammar/grammar_test_17.g");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: rule token is not defined: bruh", parser.GetError());
}

TEST(grammar, parser18) {
  Parser parser = Parser("invalid_file_path");
  ASSERT_EQ(false, parser.Parse());
  ASSERT_EQ("grammar parsing error: file not found", parser.GetError());
}
