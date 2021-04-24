#include "utils/leftRecursion.h"
namespace grammar = jucc::grammar;

grammar::Productions jucc::utils::RemoveDirectLeftRecursion(const grammar::Production &prod) {
  if (!IsRecursive(prod)) {
    return grammar::Productions{prod};
  }
  const std::string &parent = prod.GetParent();
  std::string parent_dash = prod.GetParent() + std::string(DASH);

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
      entries.push_back(parent_dash);
      parent_rules.emplace_back(grammar::Rule(entries));
    }
  }
  parent_dash_rules.push_back(grammar::Rule({std::string(grammar::EPSILON)}));
  prods[0].SetRules(parent_dash_rules);
  prods[1].SetRules(parent_rules);
  return prods;
}

bool jucc::utils::IsRecursive(const grammar::Production &prod) {
  for (const auto &rule : prod.GetRules()) {
    if (prod.GetParent() == rule.GetEntities()[0]) {
      return true;
    }
  }
  return false;
}
