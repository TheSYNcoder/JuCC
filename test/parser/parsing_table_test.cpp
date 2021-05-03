#include "parser/parsing_table.h"

#include "grammar/grammar.h"
#include "gtest/gtest.h"

using jucc::parser::ParsingTable;
namespace grammar = jucc::grammar;
namespace utils = jucc::utils;

TEST(parser, test1) {
  /**
   * Test: Context Free Grammar
   * S : a A B b
   * A : c | EPSILON
   * B : d | EPSILON
   *
   *
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"a", "A", "B", "b"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"c"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("B");
  p3.SetRules({grammar::Rule({"d"}), grammar::Rule({grammar::EPSILON})});
  grammar::Productions grammar = {p1, p2, p3};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "S");

  std::vector<std::string> terminals = {"a", "b", "c", "d", "$"};
  std::vector<std::string> non_terminals = {"A", "S", "B"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  std::pair<int, int> p;
  p = table.GetEntry("S", "a");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("A", "b");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("A", "c");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("A", "d");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("B", "b");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("B", "d");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 0);
}

TEST(parser, test2) {
  /**
   * Test: Context Free Grammar
   * S : a B | EPSILON
   * B : b C | EPSILON
   * C : c S | EPSILON
   *
   *
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"a", "B"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p2;
  p2.SetParent("B");
  p2.SetRules({grammar::Rule({"b", "C"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("C");
  p3.SetRules({grammar::Rule({"c", "S"}), grammar::Rule({grammar::EPSILON})});
  grammar::Productions grammar = {p1, p2, p3};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "S");

  std::vector<std::string> terminals = {"a", "b", "c", "$"};
  std::vector<std::string> non_terminals = {"C", "S", "B"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  std::pair<int, int> p;
  p = table.GetEntry("S", "a");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("B", "b");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("C", "c");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("S", "$");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("B", "$");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("C", "$");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 1);
}

TEST(parser, test3) {
  /**
   * Test: Context Free Grammar
   * S : a B | EPSILON
   * B : b C | EPSILON
   * C : c S | EPSILON
   *
   *
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"a", "B"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p2;
  p2.SetParent("B");
  p2.SetRules({grammar::Rule({"b", "C"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("C");
  p3.SetRules({grammar::Rule({"c", "S"}), grammar::Rule({grammar::EPSILON})});
  grammar::Productions grammar = {p1, p2, p3};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  auto follows = utils::CalcFollows(grammar, firsts, nullables, "S");

  std::vector<std::string> terminals = {"a", "b", "c", "$"};
  std::vector<std::string> non_terminals = {"C", "S", "B"};

  ParsingTable table = ParsingTable(terminals, non_terminals);
  table.SetFirsts(firsts);
  table.SetFollows(follows);
  table.SetProductions(grammar);
  table.BuildTable();

  std::pair<int, int> p;
  p = table.GetEntry("S", "a");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("B", "b");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("C", "c");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("S", "$");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("B", "$");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("C", "$");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 1);
}

TEST(parser, test4) {
  /**
   * Test: Context Free Grammar
   * S : A B
   * A : a | EPSILON
   * B : b | EPSILON
   *
   *
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A", "B"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"a"}), grammar::Rule({grammar::EPSILON})});
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
  p = table.GetEntry("S", "a");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("S", "b");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("A", "a");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("B", "b");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("S", "$");
  ASSERT_EQ(p.first, 0);
  ASSERT_EQ(p.second, 0);

  p = table.GetEntry("A", "$");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("A", "b");
  ASSERT_EQ(p.first, 1);
  ASSERT_EQ(p.second, 1);

  p = table.GetEntry("B", "$");
  ASSERT_EQ(p.first, 2);
  ASSERT_EQ(p.second, 1);
}
