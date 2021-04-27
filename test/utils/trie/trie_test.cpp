#include "grammar/grammar.h"
#include "gtest/gtest.h"
#include "utils/trie/memory_efficient_trie.h"
namespace grammar = jucc::grammar;

TEST(trie, trie1) {
  // E -> ieStSt | a | b ieS | ieStS
  grammar::Production p;
  p.SetRules({
      grammar::Rule({"i", "e", "S", "S", "t"}),
      grammar::Rule({"a"}),
      grammar::Rule({"b"}),
      grammar::Rule({"i", "e", "S"}),
      grammar::Rule({"i", "e", "S", "t", "S"}),
  });

  jucc::utils::TrieManager tm;
  tm.InsertAll(p);

  grammar::Rule rule;
  int len = 1;
  jucc::utils::TrieManager::GreedyPreorder(tm.GetMaster(), len, rule, true);
  ASSERT_EQ(rule.ToString(), "ieS");
}
