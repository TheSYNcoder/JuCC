#include "gtest/gtest.h"
#include "utils/leftFactoring.h"
#include "utils/leftRecursion.h"

namespace grammar = jucc::grammar;

TEST(utils, directLeftRecursion) {
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

TEST(jucc, MaxLenPrefixTest0) {
  // E -> ieStSt | a | b | ie | ieS | ieStP
  grammar::Production p;
  p.setRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e"}), grammar::Rule({"i", "e", "S"}), grammar::Rule({"i", "e", "S", "t", "P"})});
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).toString(), "ie");
}

TEST(jucc, MaxLenPrefixTest1) {
  // E -> ieStSt | a | b | ieStS
  grammar::Production p;
  p.setRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"i", "e", "S", "t", "S"}),
  });
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).toString(), "ieStS");
}

TEST(jucc, MaxLenPrefixTest2) {
  // no rule
  grammar::Production p;
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).toString(), "");
}

TEST(jucc, MaxLenPrefixTest3) {
  // E -> a | b | C | EPSILON
  grammar::Production p;
  p.setRules({
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"c"}),
      grammar::Rule({std::string(grammar::EPSILON)}),
  });
  auto max_len_entity = jucc::utils::LongestCommonPrefix(p);
  ASSERT_EQ(grammar::Rule(max_len_entity).toString(), "");
}

TEST(jucc, LeftFactoring0) {
  // E -> ieStSt | a | b | ieStP
  grammar::Production p;
  p.setParent("E");
  p.setRules({grammar::Rule({"i", "e", "S", "t", "S", "t"}), grammar::Rule({"a"}), grammar::Rule({"b"}),
              grammar::Rule({"i", "e", "S", "t", "P"})});

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);

  ASSERT_EQ(lf_removed.size(), 2);
  // output
  //  E -> ieStE' | a | b |
  //  E' -> St | P | epsilon
  ASSERT_EQ(lf_removed[0].getParent(), "E");
  ASSERT_EQ(lf_removed[1].getParent(), "E" + std::string(jucc::utils::DASH));

  ASSERT_EQ(lf_removed[0].getRules().size(), 3);
  ASSERT_EQ(lf_removed[1].getRules().size(), 3);

  ASSERT_EQ(lf_removed[0].getRules()[0].toString(), "ieStE'");
  ASSERT_EQ(lf_removed[0].getRules()[1].toString(), "a");
  ASSERT_EQ(lf_removed[0].getRules()[2].toString(), "b");

  ASSERT_EQ(lf_removed[1].getRules()[0].toString(), "St");
  ASSERT_EQ(lf_removed[1].getRules()[1].toString(), "P");
  ASSERT_EQ(lf_removed[1].getRules()[2].toString(), std::string(jucc::grammar::EPSILON));
}

TEST(jucc, LeftFactoring1) {
  // E -> ieStSt | a | b
  grammar::Production p;
  p.setParent("E");
  p.setRules({
      grammar::Rule({"i", "e", "S", "t", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
  });

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);
  // output E -> ieStSt | a | b

  ASSERT_EQ(lf_removed.size(), 1);
  ASSERT_EQ(lf_removed[0].getParent(), "E");

  ASSERT_EQ(lf_removed[0].getRules().size(), 3);

  ASSERT_EQ(lf_removed[0].getRules()[0].toString(), "ieStSt");
  ASSERT_EQ(lf_removed[0].getRules()[1].toString(), "a");
  ASSERT_EQ(lf_removed[0].getRules()[2].toString(), "b");
}

TEST(jucc, LeftFactoringComplex) {
  // E -> a | ab | abc | e | f
  grammar::Production p;
  p.setParent("E");
  p.setRules({grammar::Rule({"a"}), grammar::Rule({"a", "b"}), grammar::Rule({"a", "b", "c"}), grammar::Rule({"e"}),
              grammar::Rule({"f"})});

  auto lf_removed = jucc::utils::RemoveLeftFactors(p);
  // output
  //  E ->   aE' | e | f
  //  E' ->  bE'' | epsilon  //from E' -> b | bc | epsilon
  //  E'' -> epsilon | c

  ASSERT_EQ(lf_removed.size(), 3);

  ASSERT_EQ(lf_removed[0].getParent(), "E");
  ASSERT_EQ(lf_removed[1].getParent(), "E" + std::string(jucc::utils::DASH));
  ASSERT_EQ(lf_removed[2].getParent(), "E" + std::string(jucc::utils::DASH) + std::string(jucc::utils::DASH));

  ASSERT_EQ(lf_removed[0].getRules().size(), 3);
  ASSERT_EQ(lf_removed[1].getRules().size(), 2);
  ASSERT_EQ(lf_removed[1].getRules().size(), 2);

  ASSERT_EQ(lf_removed[0].getRules()[0].toString(), "aE'");
  ASSERT_EQ(lf_removed[0].getRules()[1].toString(), "e");
  ASSERT_EQ(lf_removed[0].getRules()[2].toString(), "f");

  ASSERT_EQ(lf_removed[1].getRules()[0].toString(), "bE''");
  ASSERT_EQ(lf_removed[1].getRules()[1].toString(), std::string(jucc::grammar::EPSILON));

  ASSERT_EQ(lf_removed[2].getRules()[0].toString(), std::string(jucc::grammar::EPSILON));
  ASSERT_EQ(lf_removed[2].getRules()[1].toString(), "c");
}
