#include "utils/left_factoring.h"

#include <sstream>

#include "utils/left_recursion.h"
#include "utils/trie/memory_efficient_trie.h"

namespace jucc::utils {

grammar::Productions RemoveLeftFactors(const grammar::Production &prod) {
  grammar::Productions prods;

  bool state_not_updated = false;
  auto current_prod = prod;
  while (!state_not_updated) {
    auto max_common_prefix = utils::LongestCommonPrefix(current_prod).GetEntities();
    if (max_common_prefix.empty()) {
      state_not_updated = true;
      continue;
    }

    auto parent = current_prod.GetParent();                                  // similar to E
    auto parent_dash = current_prod.GetParent() + std::string(utils::DASH);  // similar to E'

    grammar::Rules parent_rules;
    grammar::Rules parent_dash_rules;

    // insert the commonE' prod
    std::vector<std::string> common_factor_with_dash(max_common_prefix.begin(), max_common_prefix.end());
    // push E' after the common part
    common_factor_with_dash.emplace_back(parent_dash);
    parent_rules.emplace_back(grammar::Rule(common_factor_with_dash));

    bool has_epsilon_inserted = false;
    // produce two production after matching
    for (const auto &rule : current_prod.GetRules()) {
      if (!rule.HasPrefix(grammar::Rule(max_common_prefix))) {
        parent_rules.emplace_back(rule);
      } else {
        auto new_entities = std::vector<std::string>(
            rule.GetEntities().begin() + static_cast<int>(max_common_prefix.size()), rule.GetEntities().end());
        if (new_entities.empty()) {
          has_epsilon_inserted = true;
          new_entities.emplace_back(std::string(grammar::EPSILON));
        }
        parent_dash_rules.emplace_back(new_entities);
      }
    }

    if (!has_epsilon_inserted) {
      parent_dash_rules.emplace_back(std::vector<std::string>{std::string(grammar::EPSILON)});
    }

    // do this recursive
    prods.push_back(grammar::Production(parent, parent_rules));
    current_prod = grammar::Production(parent_dash, parent_dash_rules);
  }
  prods.push_back(current_prod);
  return prods;
}

grammar::Rule LongestCommonPrefix(const grammar::Production &prod) {
  TrieManager trie_manager;
  trie_manager.InsertAll(prod);

  grammar::Rule common_prefixes;
  int len = 1;
  TrieManager::GreedyPreorder(trie_manager.GetMaster(), len, common_prefixes, true);

  return common_prefixes;
}

}  // namespace jucc::utils
