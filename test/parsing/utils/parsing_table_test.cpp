#include "parsing/utils/parsing_table.h"

#include "grammar/grammar.h"
#include "gtest/gtest.h"

using jucc::parsing_table::ParsingTable;
namespace grammar = jucc::grammar;
namespace utils = jucc::utils;

TEST(parsing, test1) {
  /**
   * Test: Context Free Grammar
   * S : A S A | a B | b
   * A : B
   * B : b | EPSILON
   *
   * nullables: A, B
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A", "S", "A"}), grammar::Rule({"a", "B"}), grammar::Rule({"b"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"B"})});
  grammar::Production p3;
  p3.SetParent("B");
  p3.SetRules({grammar::Rule({"b"}), grammar::Rule({grammar::EPSILON})});
  grammar::Productions grammar = {p1, p2, p3};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "S");

  std::vector<std::string> terminals = {"a", "b", "$"};
  std::vector<std::string> non_terminals = {"A", "S", "B"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  std::pair<int, int> p;
  p = table.GetEntry("A", "b");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 0);
}
