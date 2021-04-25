#include "gtest/gtest.h"
#include "utils/left_factoring.h"
#include "utils/left_recursion.h"

namespace grammar = jucc::grammar;

TEST(utils, DirectLeftRecursion) {
  // E -> E + T | T | epsilon
  grammar::Production prod;
  prod.SetParent("E");
  auto rule1 = grammar::Rule({"E", "+", "T"});

  auto rule2 = grammar::Rule({"T"});
  auto rule3 = grammar::Rule({std::string(grammar::EPSILON)});

  prod.SetRules({rule1, rule2, rule3});

  auto prods = jucc::utils::RemoveDirectLeftRecursion(prod);
  // output E' -> +TE' | epsilon
  // E ->   TE' | epsilonE'
  ASSERT_EQ(prods[0].GetParent(), "E" + std::string(jucc::utils::DASH));
  ASSERT_EQ(prods[1].GetParent(), "E");

  auto rules_e = prods[1].GetRules();
  auto rules_e_dash = prods[0].GetRules();

  ASSERT_EQ(rules_e.size(), 2);
  ASSERT_EQ(rules_e_dash.size(), 2);

  ASSERT_EQ(rules_e_dash[0].ToString(), "+TE" + std::string(jucc::utils::DASH));
  ASSERT_EQ(rules_e_dash[1].ToString(), std::string(grammar::EPSILON));

  ASSERT_EQ(rules_e[0].ToString(), "TE" + std::string(jucc::utils::DASH));
  ASSERT_EQ(rules_e[1].ToString(), std::string(grammar::EPSILON) + "E" + std::string(jucc::utils::DASH));
}

TEST(jucc, MaxLenPrefix0) {
  // E -> ieStSt | a | b | ie | ieS | ieStP
  grammar::Production p;
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e"}), grammar::Rule({"i", "e", "S"}), grammar::Rule({"i", "e", "S", "t", "P"})});
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "ie");
}

TEST(jucc, MaxLenPrefix1) {
  // E -> ieStSt | a | b | ieStS
  grammar::Production p;
  p.SetRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"i", "e", "S", "t", "S"}),
  });
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "ieStS");
}

TEST(jucc, MaxLenPrefix2) {
  // no rule
  grammar::Production p;
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "");
}

TEST(jucc, MaxLenPrefix3) {
  // E -> a | b | C | EPSILON
  grammar::Production p;
  p.SetRules({
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"c"}),
      grammar::Rule({std::string(grammar::EPSILON)}),
  });
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).ToString(), "");
}

TEST(jucc, LeftFactoring0) {
  // E -> ieStSt | a | b | ieStP
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e", "S", "t", "P"})});

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);

  ASSERT_EQ(lf_removed.size(), 2);
  // output
  //  E -> ieStE' | a | b |
  //  E' -> St | P | epsilon
  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(jucc::utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 3);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "St");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), "P");
  ASSERT_EQ(lf_removed[1].GetRules()[2].ToString(), std::string(jucc::grammar::EPSILON));
}

TEST(jucc, LeftFactoring1) {
  // E -> ieStSt | a | b
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
  });

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);
  // output E -> ieStSt | a | b

  ASSERT_EQ(lf_removed.size(), 1);
  ASSERT_EQ(lf_removed[0].GetParent(), "E");

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "ieStSt");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "a");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "b");
}

TEST(jucc, LeftFactoringRecursive0) {
  // E -> a | ab | abc | e | f
  grammar::Production p;
  p.SetParent("E");
  p.SetRules({grammar::Rule({"a"}), grammar::Rule({"a", "b"}), grammar::Rule({"a", "b", "c"}), grammar::Rule({"e"}),
              grammar::Rule({"f"})});

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);
  // output
  //  E ->   aE' | e | f
  //  E' ->  bE'' | epsilon  //from E' -> b | bc | epsilon
  //  E'' -> epsilon | c

  ASSERT_EQ(lf_removed.size(), 3);

  ASSERT_EQ(lf_removed[0].GetParent(), "E");
  ASSERT_EQ(lf_removed[1].GetParent(), "E" + std::string(jucc::utils::DASH));
  ASSERT_EQ(lf_removed[2].GetParent(), "E" + std::string(jucc::utils::DASH) + std::string(jucc::utils::DASH));

  ASSERT_EQ(lf_removed[0].GetRules().size(), 3);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);
  ASSERT_EQ(lf_removed[1].GetRules().size(), 2);

  ASSERT_EQ(lf_removed[0].GetRules()[0].ToString(), "aE'");
  ASSERT_EQ(lf_removed[0].GetRules()[1].ToString(), "e");
  ASSERT_EQ(lf_removed[0].GetRules()[2].ToString(), "f");

  ASSERT_EQ(lf_removed[1].GetRules()[0].ToString(), "bE''");
  ASSERT_EQ(lf_removed[1].GetRules()[1].ToString(), std::string(jucc::grammar::EPSILON));

  ASSERT_EQ(lf_removed[2].GetRules()[0].ToString(), std::string(jucc::grammar::EPSILON));
  ASSERT_EQ(lf_removed[2].GetRules()[1].ToString(), "c");
}
