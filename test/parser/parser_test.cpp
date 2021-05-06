#include "parser/parser.h"

#include "grammar/grammar.h"
#include "gtest/gtest.h"
#include "parser/parsing_table.h"
#include "utils/utils.h"

using jucc::parser::Parser;
using jucc::parser::ParsingTable;
namespace grammar = jucc::grammar;
namespace utils = jucc::utils;

// Consult - for LL1 parsing
// https://www.rose-hulman.edu/class/csse/csse404/schedule/day31/ErrorRecovery.pdf

TEST(parser, Parser1) {
  /**
   * Grammar:
   * E -> TE'
   * E' -> +TE' | EPSILON
   * T -> FT'
   * T' -> *FT | EPSILON
   * F -> id | ( E )
   */
  grammar::Production p1;
  p1.SetParent("E");
  p1.SetRules({grammar::Rule({"T", "E'"})});
  grammar::Production p2;
  p2.SetParent("E'");
  p2.SetRules({grammar::Rule({"+", "T", "E'"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("T");
  p3.SetRules({grammar::Rule({"F", "T'"})});
  grammar::Production p4;
  p4.SetParent("T'");
  p4.SetRules({grammar::Rule({"*", "F", "T'"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p5;
  p5.SetParent("F");
  p5.SetRules({grammar::Rule({"id"}), grammar::Rule({"(", "E", ")"})});

  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "E");

  std::vector<std::string> terminals = {"id", "+", "*", "(", ")"};
  std::vector<std::string> non_terminals = {"E", "E'", "T", "T'", "F"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  Parser pars = Parser();

  std::vector<std::string> input = {"+", "id", "*", "+", "id"};
  pars.SetInputString(input);
  pars.SetStartSymbol("E");
  pars.ResetParsing();
  pars.SetParsingTable(table);
  pars.ParseNextStep();

  std::vector<int> history;
  std::vector<std::string> parser_errors;
  history = pars.GetProductionHistory();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 1);
  // Stack - T E' $
  ASSERT_EQ(history.size(), 1);
  ASSERT_EQ(history[0], 0);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();
  // Stack - F T' E' $
  ASSERT_EQ(history.size(), 2);
  ASSERT_EQ(history[1], 200);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();
  // Stack - id T' E' $
  ASSERT_EQ(history.size(), 3);
  ASSERT_EQ(history[2], 400);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - T' E' $

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - * F T' E' $

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  ASSERT_EQ(history.size(), 4);
  ASSERT_EQ(history[3], 300);

  // Stack - F T' E' $
  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 2);

  // Stack - T' E' $
  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - E' $

  ASSERT_EQ(history.size(), 5);
  ASSERT_EQ(history[4], 301);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - + T E' $
  ASSERT_EQ(history.size(), 6);
  ASSERT_EQ(history[5], 100);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - T E' $

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - F T ' E' $
  ASSERT_EQ(history.size(), 7);
  ASSERT_EQ(history[6], 200);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - id T' E' $
  ASSERT_EQ(history.size(), 8);
  ASSERT_EQ(history[7], 400);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - T' E' $

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  // Stack - E' $
  ASSERT_EQ(history.size(), 9);
  ASSERT_EQ(history[8], 301);

  pars.ParseNextStep();
  history = pars.GetProductionHistory();

  ASSERT_EQ(history.size(), 10);
  ASSERT_EQ(history[9], 101);

  // Stack - $

  ASSERT_TRUE(pars.IsComplete());
}

TEST(parser, Parser2) {
  grammar::Parser grammar_parser = grammar::Parser("../test/parser/grammar.g");
  ASSERT_TRUE(grammar_parser.Parse());

  grammar::Productions raw_productions = grammar_parser.GetProductions();
  grammar::Productions productions = utils::RemoveAllPossibleAmbiguity(raw_productions);

  /* Calculate first and follows to build the LL(1) parsing table */
  auto nullables = utils::CalcNullables(productions);
  auto firsts = utils::CalcFirsts(productions, nullables);
  auto follows = utils::CalcFollows(productions, firsts, nullables, grammar_parser.GetStartSymbol());
  auto terminals = grammar_parser.GetTerminals();
  auto non_terminals = utils::GetAllNonTerminals(productions);

  ParsingTable parsing_table = ParsingTable(terminals, non_terminals);
  parsing_table.SetFirsts(firsts);
  parsing_table.SetFollows(follows);
  parsing_table.SetProductions(productions);
  parsing_table.BuildTable();
  ASSERT_EQ(parsing_table.GetErrors().size(), 0);
  std::vector<std::string> input_string = {"int", "main", "(", ")", "{", "if", "integer_constant", ";", "}"};
  Parser pars = Parser();

  pars.SetInputString(input_string);
  pars.SetStartSymbol(grammar_parser.GetStartSymbol());
  pars.ResetParsing();
  pars.SetParsingTable(parsing_table);

  std::vector<std::string> parser_errors;

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 0);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 1);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 2);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 3);

  pars.ParseNextStep();
  parser_errors = pars.GetParserErrors();
  ASSERT_EQ(parser_errors.size(), 4);

  ASSERT_TRUE(pars.IsComplete());
}
