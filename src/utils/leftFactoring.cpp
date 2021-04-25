#include "utils/leftFactoring.h"

#include <sstream>

#include "trie/memoryEfficientTrie.h"
#include "utils/leftRecursion.h"

namespace jucc {

grammar::Productions jucc::utils::RemoveLeftFactors(const grammar::Production &prod) {
  grammar::Productions prods;

  bool state_not_updated = false;
  auto current_prod = prod;
  while (!state_not_updated) {
    auto max_common_prefix = utils::LongestCommonPrefix(current_prod);
    if (max_common_prefix.empty()) {
      state_not_updated = true;
      continue;
    }

    auto parent = current_prod.GetParent();                                  // similar to E
    auto parent_dash = current_prod.GetParent() + std::string(utils::DASH);  // similar to E'

    grammar::Rules parent_rules;
    grammar::Rules parent_dash_rules;

    // insert the commonE' prod
    grammar::Entity common_factor_with_dash(max_common_prefix.begin(), max_common_prefix.end());
    // push E' after the common part
    common_factor_with_dash.emplace_back(parent_dash);
    parent_rules.emplace_back(grammar::Rule(common_factor_with_dash));

    bool has_epsilon_inserted = false;
    // produce two production after matching
    for (const auto &rule : current_prod.GetRules()) {
      if (!rule.HasPrefix(max_common_prefix)) {
        parent_rules.emplace_back(rule);
      } else {
        auto new_entities = grammar::Entity(rule.GetEntities().begin() + static_cast<int>(max_common_prefix.size()),
                                            rule.GetEntities().end());
        if (new_entities.empty()) {
          has_epsilon_inserted = true;
          new_entities.emplace_back(std::string(grammar::EPSILON));
        }
        parent_dash_rules.push_back(grammar::Rule(new_entities));
      }
    }
    if (!has_epsilon_inserted) {
      parent_dash_rules.emplace_back(grammar::Entity{std::string(grammar::EPSILON)});
    }
    // do this recursive
    prods.push_back(grammar::Production(parent, parent_rules));
    current_prod = grammar::Production(parent_dash, parent_dash_rules);
  }
  prods.push_back(current_prod);
  return prods;
}

grammar::Entity jucc::utils::LongestCommonPrefix(const grammar::Production &prod) {
  jucc::TrieManager trie_manager;
  trie_manager.InsertAll(prod);
  grammar::Entity common_prefixes;
  int len = 1;
  TrieManager::GreedyPreorder(trie_manager.GetMaster(), len, common_prefixes, true);

  return common_prefixes;
}
}  // namespace jucc
