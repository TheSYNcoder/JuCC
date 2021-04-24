#include "gtest/gtest.h"
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
