#include "utils/left_recursion.h"

#include <algorithm>
namespace jucc::utils {

grammar::Productions RemoveDirectLeftRecursion(const grammar::Production &prod) {
  if (!IsRecursive(prod)) {
    return grammar::Productions{prod};
  }
  const std::string &parent = prod.GetParent();
  std::string parent_dash = prod.GetParent() + std::string(DASH) + std::string(DASHAT);

  grammar::Productions prods(2);
  prods[0].SetParent(parent_dash);
  prods[1].SetParent(parent);

  auto parent_rules = prods[0].GetRules();
  auto parent_dash_rules = prods[1].GetRules();

  for (const auto &rule : prod.GetRules()) {
    auto entries = rule.GetEntities();
    if (entries[0] == parent) {
      auto new_entries = std::vector<std::string>(entries.begin() + 1, entries.end());
      new_entries.push_back(parent_dash);
      parent_dash_rules.emplace_back(grammar::Rule(new_entries));
    } else {
      if (!entries.empty() && entries.back() == std::string(grammar::EPSILON)) {
        entries.pop_back();
      }
      entries.push_back(parent_dash);
      parent_rules.emplace_back(grammar::Rule(entries));
    }
  }
  parent_dash_rules.push_back(grammar::Rule({std::string(grammar::EPSILON)}));
  prods[0].SetRules(parent_dash_rules);
  prods[1].SetRules(parent_rules);
  return prods;
}

bool IsRecursive(const grammar::Production &prod) {
  const auto &rules = prod.GetRules();

  return static_cast<bool>(std::any_of(rules.begin(), rules.end(),
                                       [&](const auto &rule) { return prod.GetParent() == rule.GetEntities()[0]; }));
}

}  // namespace jucc::utils
