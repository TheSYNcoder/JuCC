#include "parsing/parsing.h"

#include "grammar/grammar.h"
#include "gtest/gtest.h"
#include "parsing/utils/parsing_table.h"

using jucc::parsing::Parsing;
using jucc::parsing_table::ParsingTable;
namespace grammar = jucc::grammar;
namespace utils = jucc::utils;

TEST(parsing, parsing1) {
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
  p5.SetRules({grammar::Rule({"id"}), grammar::Rule({grammar::EPSILON})});

  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "E");

  std::vector<std::string> terminals = {"id", "+", "*", "(", ")", "$"};
  std::vector<std::string> non_terminals = {"E", "E'", "T", "T'", "F"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  Parsing pars = Parsing();

  std::vector<std::string> input = {")", "id", "*", "+", "id"};
  pars.Init();
  pars.SetInputString(input);
  pars.SetStartSymbol("E");
  pars.SetParsingTable(table);
  pars.ParseNextStep();

  std::vector<int> history;
  history = pars.GetProductionHistory();

  ASSERT_EQ(history.size(), 1);
}
