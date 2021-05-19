#include "utils/utils.h"

#include "gtest/gtest.h"

namespace grammar = jucc::grammar;
namespace utils = jucc::utils;

TEST(utils, DirectLeftRecursion0) {
  // E -> E + T | T | epsilon
  grammar::Production prod;
  prod.SetParent("E");
  auto rule1 = grammar::Rule({"E", "+", "T"});

  auto rule2 = grammar::Rule({"T"});
  auto rule3 = grammar::Rule({std::string(grammar::EPSILON)});

  prod.SetRules({rule1, rule2, rule3});
  auto prods = utils::RemoveDirectLeftRecursion(prod);
  // output E'@ -> +TE'@ | epsilon
  // E ->   TE'@ | E'
  ASSERT_EQ(prods[0].GetParent(), "E" + std::string(utils::DASH) + std::string(utils::DASHAT));
  ASSERT_EQ(prods[1].GetParent(), "E");

  auto rules_e = prods[1].GetRules();
  auto rules_e_dash = prods[0].GetRules();

  ASSERT_EQ(rules_e.size(), 2);
  ASSERT_EQ(rules_e_dash.size(), 2);

  ASSERT_EQ(rules_e_dash[0].ToString(), "+TE" + std::string(utils::DASH) + std::string(utils::DASHAT));
  ASSERT_EQ(rules_e_dash[1].ToString(), std::string(grammar::EPSILON));

  ASSERT_EQ(rules_e[0].ToString(), "TE" + std::string(utils::DASH) + std::string(utils::DASHAT));
  ASSERT_EQ(rules_e[1].ToString(), "E" + std::string(utils::DASH) + std::string(utils::DASHAT));
}

TEST(utils, DirectLeftRecursion1) {
  // E -> epsilon
  grammar::Production prod;
  prod.SetParent("E");

  auto rule1 = grammar::Rule({std::string(grammar::EPSILON)});

  prod.SetRules({rule1});
  auto prods = utils::RemoveDirectLeftRecursion(prod);

  // E ->  epsilon
  ASSERT_EQ(prods.size(), 1);
  ASSERT_EQ(prods[0].GetParent(), "E");

  auto rules_e = prods[0].GetRules();
  ASSERT_EQ(rules_e.size(), 1);

  ASSERT_EQ(rules_e[0].ToString(), std::string(grammar::EPSILON));
}

TEST(utils, MaxLenPrefix0) {
  // E -> ieStSt | a | b | ie | ieS | ieStP
  grammar::Production p;
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e"}), grammar::Rule({"i", "e", "S"}), grammar::Rule({"i", "e", "S", "t", "P"})});
  auto max_len_entity = utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "ie");
}

TEST(utils, MaxLenPrefix1) {
  // E -> ieStSt | a | b | ieStS
  grammar::Production p;
  p.SetRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"i", "e", "S", "t", "S"}),
  });
  auto max_len_entity = utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "ieStS");
}

TEST(utils, MaxLenPrefix2) {
  // no rule
  grammar::Production p;
  auto max_len_entity = utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "");
}

TEST(utils, MaxLenPrefix3) {
  // E -> a | b | C | EPSILON
  grammar::Production p;
  p.SetRules({
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"c"}),
      grammar::Rule({std::string(grammar::EPSILON)}),
  });
  auto max_len_entity = utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "");
}

TEST(utils, LeftFactoring0) {
  // E -> ieStSt | a | b | ieStP
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e", "S", "t", "P"})});

  auto lf_removed = utils::RemoveLeftFactors(p);

  ASSERT_EQ(lf_removed.size(), 2);
  // output
  //  E -> ieStE' | a | b |
  //  E' -> St | P
  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "St");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), "P");
}

TEST(utils, LeftFactoring1) {
  // E -> ieStSt | a | b | ieSt
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e", "S", "t"})});

  auto lf_removed = utils::RemoveLeftFactors(p);

  ASSERT_EQ(lf_removed.size(), 2);
  // output
  //  E -> ieStE' | a | b |
  //  E' -> St | epsilon
  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "St");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), std::string(grammar::EPSILON));
}

TEST(utils, LeftFactoring2) {
  // E -> ieStSt | a | b
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
  });

  auto lf_removed = utils::RemoveLeftFactors(p);
  // output E -> ieStSt | a | b

  ASSERT_EQ(lf_removed.size(), 1);
  ASSERT_EQ(lf_removed[0].GetParent(), "E");

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStSt");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");
}

TEST(utils, LeftFactoringRecursive0) {
  // E -> a | ab | abc | e | f
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"a"}), grammar::Rule({"a", "b"}), grammar::Rule({"a", "b", "c"}), grammar::Rule({"e"}),
              grammar::Rule({"f"})});

  auto lf_removed = utils::RemoveLeftFactors(p);
  // output
  //  E ->   aE' | e | f
  //  E' ->  bE'' | epsilon  //from E' -> b | bc | epsilon
  //  E'' -> epsilon | c

  ASSERT_EQ(lf_removed.size(), 3);

  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(utils::DASH));
  ASSERT_EQ(lf_removed[2].GetParent(), "E" + std::string(utils::DASH) + std::string(utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "aE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "e");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "f");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "bE''");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), std::string(grammar::EPSILON));

  ASSERT_EQ(lf_removed[2].GetRules()[0].ToString(), std::string(grammar::EPSILON));
  ASSERT_EQ(lf_removed[2].GetRules()[1].ToString(), "c");
}

TEST(utils, CalcNullables0) {
  /**
   * Test: Context Free Grammar
   * S -> A B A C
   * A -> a A | EPSILON
   * B -> b B | EPSILON
   * C -> c
   *
   * nullables: A, B
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A", "B", "A", "C"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"a", "A"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("B");
  p3.SetRules({grammar::Rule({"b", "B"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p4;
  p4.SetParent("C");
  p4.SetRules({grammar::Rule({"c"})});

  grammar::Productions grammar = {p1, p2, p3, p4};

  std::unordered_map<std::string, bool> res = utils::CalcNullables(grammar);
  ASSERT_EQ(8, res.size());
  ASSERT_FALSE(res.at("a"));
  ASSERT_FALSE(res.at("b"));
  ASSERT_FALSE(res.at("c"));
  ASSERT_TRUE(res.at(grammar::EPSILON));
  ASSERT_FALSE(res.at("S"));
  ASSERT_TRUE(res.at("A"));
  ASSERT_TRUE(res.at("B"));
  ASSERT_FALSE(res.at("C"));
}

TEST(utils, CalcNullables1) {
  /**
   * Test: Context Free Grammar
   * S -> A B
   * A -> a A A | EPSILON
   * B -> b B B | EPSILON
   *
   * nullables: S, A, B
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A", "B"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"a", "A", "A"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p3;
  p3.SetParent("B");
  p3.SetRules({grammar::Rule({"b", "B", "B"}), grammar::Rule({grammar::EPSILON})});

  grammar::Productions grammar = {p1, p2, p3};

  std::unordered_map<std::string, bool> res = utils::CalcNullables(grammar);
  ASSERT_EQ(6, res.size());
  ASSERT_FALSE(res.at("a"));
  ASSERT_FALSE(res.at("b"));
  ASSERT_TRUE(res.at(grammar::EPSILON));
  ASSERT_TRUE(res.at("S"));
  ASSERT_TRUE(res.at("A"));
  ASSERT_TRUE(res.at("B"));
}

TEST(utils, CalcNullables2) {
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

  std::unordered_map<std::string, bool> res = utils::CalcNullables(grammar);
  ASSERT_EQ(6, res.size());
  ASSERT_FALSE(res.at("a"));
  ASSERT_FALSE(res.at("b"));
  ASSERT_TRUE(res.at(grammar::EPSILON));
  ASSERT_FALSE(res.at("S"));
  ASSERT_TRUE(res.at("A"));
  ASSERT_TRUE(res.at("B"));
}

TEST(utils, CalcFirsts0) {
  /**
   * Test: Context Free Grammar
   * S : a B D h
   * B : c C
   * C : b C | EPSILON
   * D : E F
   * E : g | EPSILON
   * F : f | EPSILON
   *
   * First(S) = { a }
   * First(B) = { c }
   * First(C) = { b , EPSILON }
   * First(D) = { g , f , EPSILON }
   * First(E) = { g , EPSILON }
   * First(F) = { f , EPSILON }
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"a", "B", "D", "h"})});
  grammar::Production p2;
  p2.SetParent("B");
  p2.SetRules({grammar::Rule({"c", "C"})});
  grammar::Production p3;
  p3.SetParent("C");
  p3.SetRules({grammar::Rule({"b", "C"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p4;
  p4.SetParent("D");
  p4.SetRules({grammar::Rule({"E", "F"})});
  grammar::Production p5;
  p5.SetParent("E");
  p5.SetRules({grammar::Rule({"g"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p6;
  p6.SetParent("F");
  p6.SetRules({grammar::Rule({"f"}), grammar::Rule({grammar::EPSILON})});

  grammar::Productions grammar = {p1, p2, p3, p4, p5, p6};

  utils::SymbolsMap res = utils::CalcFirsts(grammar, utils::CalcNullables(grammar));
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(6, res.size());
  ASSERT_EQ(res.at("S"), std::vector<std::string>({"a"}));
  ASSERT_EQ(res.at("B"), std::vector<std::string>({"c"}));
  ASSERT_EQ(res.at("C"), std::vector<std::string>({grammar::EPSILON, "b"}));
  ASSERT_EQ(res.at("D"), std::vector<std::string>({grammar::EPSILON, "f", "g"}));
  ASSERT_EQ(res.at("E"), std::vector<std::string>({grammar::EPSILON, "g"}));
  ASSERT_EQ(res.at("F"), std::vector<std::string>({grammar::EPSILON, "f"}));
}

TEST(utils, CalcFirsts1) {
  /**
   * Test: Context Free Grammar
   * S : A
   * A : a B A'
   * A' : d A' | EPSILON
   * B : b
   * C : g
   *
   * First(S) = { a }
   * First(A) = { a }
   * First(A') = { d , EPSILON }
   * First(B) = { b }
   * First(C) = { g }
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"a", "B", "A'"})});
  grammar::Production p3;
  p3.SetParent("A'");
  p3.SetRules({grammar::Rule({"d", "A'"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p4;
  p4.SetParent("B");
  p4.SetRules({grammar::Rule({"b"})});
  grammar::Production p5;
  p5.SetParent("C");
  p5.SetRules({grammar::Rule({"g"})});
  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  utils::SymbolsMap res = utils::CalcFirsts(grammar, utils::CalcNullables(grammar));
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(5, res.size());
  ASSERT_EQ(res.at("S"), std::vector<std::string>({"a"}));
  ASSERT_EQ(res.at("A"), std::vector<std::string>({"a"}));
  ASSERT_EQ(res.at("A'"), std::vector<std::string>({grammar::EPSILON, "d"}));
  ASSERT_EQ(res.at("B"), std::vector<std::string>({"b"}));
  ASSERT_EQ(res.at("C"), std::vector<std::string>({"g"}));
}

TEST(utils, CalcFirsts2) {
  /**
   * Test: Context Free Grammar
   * E : T E'
   * E' : + T E' | EPSILON
   * T : F T'
   * T' : * F T' | EPSILON
   * F : ( E ) | id
   *
   * First(E) = { ( , id }
   * First(E') = { + , EPSILON }
   * First(T) = { ( , id }
   * First(T') = { * , EPSILON }
   * First(F) = { ( , id }
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
  p5.SetRules({grammar::Rule({"(", "E", ")"}), grammar::Rule({"id"})});
  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  utils::SymbolsMap res = utils::CalcFirsts(grammar, utils::CalcNullables(grammar));
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(5, res.size());
  ASSERT_EQ(res.at("E"), std::vector<std::string>({"(", "id"}));
  ASSERT_EQ(res.at("E'"), std::vector<std::string>({"+", grammar::EPSILON}));
  ASSERT_EQ(res.at("T"), std::vector<std::string>({"(", "id"}));
  ASSERT_EQ(res.at("T'"), std::vector<std::string>({"*", grammar::EPSILON}));
  ASSERT_EQ(res.at("F"), std::vector<std::string>({"(", "id"}));
}

TEST(utils, CalcFirsts3) {
  /**
   * A -> C a
   * B -> C b
   * C -> B c | EPSILON
   *
   * First(A) = { a , b }
   * First(B) = { b }
   * First(C) = { EPSILON , b}
   */
  grammar::Production p1;
  p1.SetParent("A");
  p1.SetRules({grammar::Rule({"C", "a"})});
  grammar::Production p2;
  p2.SetParent("B");
  p2.SetRules({grammar::Rule({"C", "b"})});
  grammar::Production p3;
  p3.SetParent("C");
  p3.SetRules({grammar::Rule({"B", "c"}), grammar::Rule({grammar::EPSILON})});
  grammar::Productions grammar = {p1, p2, p3};

  utils::SymbolsMap res = utils::CalcFirsts(grammar, utils::CalcNullables(grammar));
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(3, res.size());
  ASSERT_EQ(res.at("A"), std::vector<std::string>({"a", "b"}));
  ASSERT_EQ(res.at("B"), std::vector<std::string>({"b"}));
  ASSERT_EQ(res.at("C"), std::vector<std::string>({grammar::EPSILON, "b"}));
}

TEST(utils, CalcFollows0) {
  /**
   * Test: Context Free Grammar
   * S : a B D h
   * B : c C
   * C : b C | EPSILON
   * D : E F
   * E : g | EPSILON
   * F : f | EPSILON
   *
   * Follow(S) = { $ }
   * Follow(B) = { g , f , h }
   * Follow(C) = { g , f , h }
   * Follow(D) = { h }
   * Follow(E) = { f , h }
   * Follow(F) = { h }
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"a", "B", "D", "h"})});
  grammar::Production p2;
  p2.SetParent("B");
  p2.SetRules({grammar::Rule({"c", "C"})});
  grammar::Production p3;
  p3.SetParent("C");
  p3.SetRules({grammar::Rule({"b", "C"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p4;
  p4.SetParent("D");
  p4.SetRules({grammar::Rule({"E", "F"})});
  grammar::Production p5;
  p5.SetParent("E");
  p5.SetRules({grammar::Rule({"g"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p6;
  p6.SetParent("F");
  p6.SetRules({grammar::Rule({"f"}), grammar::Rule({grammar::EPSILON})});

  grammar::Productions grammar = {p1, p2, p3, p4, p5, p6};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  utils::SymbolsMap res = utils::CalcFollows(grammar, firsts, nullables, "S");
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(6, res.size());
  ASSERT_EQ(res.at("S"), std::vector<std::string>({utils::STRING_ENDMARKER}));
  ASSERT_EQ(res.at("B"), std::vector<std::string>({"f", "g", "h"}));
  ASSERT_EQ(res.at("C"), std::vector<std::string>({"f", "g", "h"}));
  ASSERT_EQ(res.at("D"), std::vector<std::string>({"h"}));
  ASSERT_EQ(res.at("E"), std::vector<std::string>({"f", "h"}));
  ASSERT_EQ(res.at("F"), std::vector<std::string>({"h"}));
}

TEST(utils, CalcFollow1) {
  /**
   * Test: Context Free Grammar
   * S : A
   * A : a B A'
   * A' : d A' | EPSILON
   * B : b
   * C : g
   *
   * Follow(S) = { $ }
   * Follow(A) = { $ }
   * Follow(A') = { $ }
   * Follow(B) = { d , $ }
   * Follow(C) = { }
   */
  grammar::Production p1;
  p1.SetParent("S");
  p1.SetRules({grammar::Rule({"A"})});
  grammar::Production p2;
  p2.SetParent("A");
  p2.SetRules({grammar::Rule({"a", "B", "A'"})});
  grammar::Production p3;
  p3.SetParent("A'");
  p3.SetRules({grammar::Rule({"d", "A'"}), grammar::Rule({grammar::EPSILON})});
  grammar::Production p4;
  p4.SetParent("B");
  p4.SetRules({grammar::Rule({"b"})});
  grammar::Production p5;
  p5.SetParent("C");
  p5.SetRules({grammar::Rule({"g"})});
  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  utils::SymbolsMap res = utils::CalcFollows(grammar, firsts, nullables, "S");
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(5, res.size());
  ASSERT_EQ(res.at("S"), std::vector<std::string>({utils::STRING_ENDMARKER}));
  ASSERT_EQ(res.at("A"), std::vector<std::string>({utils::STRING_ENDMARKER}));
  ASSERT_EQ(res.at("A'"), std::vector<std::string>({utils::STRING_ENDMARKER}));
  ASSERT_EQ(res.at("B"), std::vector<std::string>({utils::STRING_ENDMARKER, "d"}));
  ASSERT_EQ(res.at("C"), std::vector<std::string>({}));
}

TEST(utils, CalcFollows2) {
  /**
   * Test: Context Free Grammar
   * E : T E'
   * E' : + T E' | EPSILON
   * T : F T'
   * T' : * F T' | EPSILON
   * F : ( E ) | id
   *
   * Follow(E) = { $ , ) }
   * Follow(E') = { $ , ) }
   * Follow(T) = { + , $ , ) }
   * Follow(T') = { + , $ , ) }
   * Follow(F) = { * , + , $ , ) }
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
  p5.SetRules({grammar::Rule({"(", "E", ")"}), grammar::Rule({"id"})});
  grammar::Productions grammar = {p1, p2, p3, p4, p5};

  auto nullables = utils::CalcNullables(grammar);
  auto firsts = utils::CalcFirsts(grammar, nullables);
  utils::SymbolsMap res = utils::CalcFollows(grammar, firsts, nullables, "E");
  for (auto &elem : res) {
    sort(elem.second.begin(), elem.second.end());
  }

  ASSERT_EQ(5, res.size());
  ASSERT_EQ(res.at("E"), std::vector<std::string>({utils::STRING_ENDMARKER, ")"}));
  ASSERT_EQ(res.at("E'"), std::vector<std::string>({utils::STRING_ENDMARKER, ")"}));
  ASSERT_EQ(res.at("T"), std::vector<std::string>({utils::STRING_ENDMARKER, ")", "+"}));
  ASSERT_EQ(res.at("T'"), std::vector<std::string>({utils::STRING_ENDMARKER, ")", "+"}));
  ASSERT_EQ(res.at("F"), std::vector<std::string>({utils::STRING_ENDMARKER, ")", "*", "+"}));
}

TEST(utils, RemoveAllAmbiguity0) {
  // E -> ieStSt | a | b | ieSt
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e", "S", "t"})});

  auto lf_removed = utils::RemoveAllPossibleAmbiguity({p});
  auto non_terminals = utils::GetAllNonTerminals(lf_removed);

  ASSERT_EQ(lf_removed.size(), 2);
  // output
  //  E -> ieStE' | a | b |
  //  E' -> St | epsilon
  ASSERT_EQ(non_terminals.size(), 2);
  ASSERT_EQ(non_terminals[0], "E");
  ASSERT_EQ(non_terminals[1], "E'");
  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "St");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), std::string(grammar::EPSILON));
}
