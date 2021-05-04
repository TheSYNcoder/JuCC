#include "utils/utils.h"

#include <algorithm>

namespace jucc::utils {
grammar::Productions RemoveAllPossibleAmbiguity(const grammar::Productions &prods) {
  grammar::Productions clean;
  for (const auto &prod : prods) {
    for (const grammar::Production &lr_free : RemoveDirectLeftRecursion(prod)) {
      // non ambiguous grammars
      grammar::Productions nag = RemoveLeftFactors(lr_free);
      clean.insert(clean.end(), nag.begin(), nag.end());
    }
  }

  return clean;
}

std::vector<std::string> GetAllNonTerminals(const grammar::Productions &prods) {
  std::vector<std::string> non_terminals;
  for (const auto &prod : prods) {
    non_terminals.push_back(prod.GetParent());
  }

  return non_terminals;
}

std::vector<std::string> GetAllTerminals(const grammar::Productions &prods) {
  std::vector<std::string> terminals;
  for (const auto &production : prods) {
    for (const auto &rule : production.GetRules()) {
      for (const auto &symbol : rule.GetEntities()) {
        // if symbol is not present as a parent in grammar, then it is a terminal
        // EPSILION is ignored
        if (!grammar::HasParent(prods, symbol) && symbol != std::string(grammar::EPSILON)) {
          terminals.push_back(symbol);
        }
      }
    }
  }

  terminals.erase(std::unique(terminals.begin(), terminals.end()), terminals.end());
  return terminals;
}

}  // namespace jucc::utils
