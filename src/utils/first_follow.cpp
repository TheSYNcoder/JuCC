#include "utils/first_follow.h"

#include <functional>

#include "utils/left_factoring.h"
#include "utils/left_recursion.h"

// grammar::Productions getModifiedGrammar(const grammar::Productions &prods) {
//   grammar::Productions cache;
//   for (const auto &prod : prods) {
//     for (const auto &nag : jucc::utils::RemoveDirectLeftRecursion(prod)) {
//       auto micro_nag = jucc::utils::RemoveLeftFactors(nag);
//       cache.insert(cache.end(), micro_nag.begin(), micro_nag.end());
//     }
//   }
//   return cache;
// }

std::unordered_map<std::string, bool> CalcNullables(const grammar::Productions &augmented_grammar) {
  std::unordered_map<std::string, bool> nullables;
  // set all terminals to not nullable
  for (const auto &production : augmented_grammar) {
    for (const auto &rule : production.GetRules()) {
      for (const auto &symbol : rule.GetEntities()) {
        if (!SearchParent(augmented_grammar, symbol)) {
          nullables[symbol] = false;
        }
      }
    }
  }

  // EPSILON is nullable by defination
  if (nullables.find(std::string(grammar::EPSILON)) != nullables.end()) {
    nullables[std::string(grammar::EPSILON)] = true;
  }

  // recursively compute for all non terminals
  std::function<bool(const std::string &, std::vector<std::string>)> calc_recursive;
  calc_recursive = [&](const std::string &key, std::vector<std::string> path) {
    if (find(path.begin(), path.end(), key) != path.end()) {
      return false;
    }

    path.push_back(key);
    if (nullables.find(key) != nullables.end()) {
      return nullables[key];
    }

    for (const auto &rule : GetRulesForParent(augmented_grammar, key)) {
      std::vector<std::string>::const_iterator symbol_itr;
      for (symbol_itr = rule.GetEntities().begin(); symbol_itr != rule.GetEntities().end(); symbol_itr++) {
        if (!calc_recursive(*symbol_itr, path)) {
          break;
        }
      }

      if (symbol_itr == rule.GetEntities().end()) {
        nullables[key] = true;
        return true;
      }
    }

    nullables[key] = false;
    return false;
  };

  for (const auto &production : augmented_grammar) {
    calc_recursive(production.GetParent(), std::vector<std::string>(0));
  }

  return nullables;
}

std::unordered_map<std::string, std::vector<std::string>> CalcFirsts(
    const grammar::Productions &augmented_grammar, const std::unordered_map<std::string, bool> &nullables) {
  std::unordered_map<std::string, std::vector<std::string>> firsts;
  bool finished = false;
  for (const auto &production : augmented_grammar) {
    for (const auto &rules : production.GetRules()) {
      for (const auto &symbol : rules.GetEntities()) {
        if (SearchParent(augmented_grammar, symbol)) {
          firsts.insert(make_pair(symbol, std::vector<std::string>(1, symbol)));
        }
      }
    }
  }

  std::function<std::vector<std::string>(const std::string &, std::vector<std::string>)> calc_recursive;
  calc_recursive = [&](const std::string &key, std::vector<std::string> path) {
    if (!SearchParent(augmented_grammar, key)) {
      return firsts[key];
    }

    if (find(path.begin(), path.end(), key) != path.end()) {
      return firsts[key];
    }

    path.push_back(key);
    if (firsts.find(key) == firsts.end()) {
      firsts.insert(make_pair(key, std::vector<std::string>(0)));
      finished = false;
    }

    for (const auto &rules : GetRulesForParent(augmented_grammar, key)) {
      std::vector<std::string>::const_iterator symbol_itr;
      for (symbol_itr = rules.GetEntities().begin(); symbol_itr != rules.GetEntities().end(); symbol_itr++) {
        std::vector<std::string> first = calc_recursive(*symbol_itr, path);
        for (const auto &der : first) {
          if (der != "E" && find(firsts[key].begin(), firsts[key].end(), der) == firsts[key].end()) {
            firsts[key].push_back(der);
            finished = false;
          }
        }

        if (!nullables.at(*symbol_itr)) {
          break;
        }
      }

      if (symbol_itr == rules.GetEntities().end() &&
          find(firsts[key].begin(), firsts[key].end(), std::string(grammar::EPSILON)) == firsts[key].end()) {
        firsts[key].push_back(std::string(grammar::EPSILON));
      }
    }

    return firsts[key];
  };

  while (!finished) {
    finished = true;
    for (const auto &production : augmented_grammar) {
      calc_recursive(production.GetParent(), std::vector<std::string>(0));
    }
  }

  for (const auto &production : augmented_grammar) {
    sort(firsts[production.GetParent()].begin(), firsts[production.GetParent()].end());
  }

  return firsts;
}

std::unordered_map<std::string, std::vector<std::string>> CalcFollows(
    const grammar::Productions &augmented_grammar, const std::unordered_map<std::string, bool> nullables,
    const std::string &start_symbol) {
  std::unordered_map<std::string, std::vector<std::string>> follows;
  std::unordered_map<std::string, std::vector<std::string>> firsts = CalcFirsts(augmented_grammar, nullables);
  bool finished = false;
  for (const auto &production : augmented_grammar) {
    if (production.GetParent() == start_symbol) {
      follows.insert(make_pair(production.GetParent(), std::vector<std::string>(1, "$")));
    } else {
      follows.insert(make_pair(production.GetParent(), std::vector<std::string>(0)));
    }
  }

  std::function<void(const std::string &, const std::vector<std::string> &)> calc_recursive;
  calc_recursive = [&](const std::string &key, const std::vector<std::string> &path) {
    for (const auto &rule : GetRulesForParent(augmented_grammar, key)) {
      std::vector<std::string>::const_iterator symbol_itr;
      std::vector<std::string>::const_iterator next_itr;
      for (symbol_itr = rule.GetEntities().begin(); symbol_itr != rule.GetEntities().end(); symbol_itr++) {
        std::string mid = *symbol_itr;
        if (SearchParent(augmented_grammar, key)) {
          for (next_itr = symbol_itr + 1; next_itr != rule.GetEntities().end(); next_itr++) {
            for (const auto &der : firsts[*next_itr]) {
              if (der != std::string(grammar::EPSILON) &&
                  find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                follows[mid].push_back(der);
                finished = false;
              }
            }

            if (!nullables.at(*next_itr)) {
              break;
            }
          }

          if (next_itr == rule.GetEntities().end()) {
            for (const auto &der : follows[key]) {
              if (find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                follows[mid].push_back(der);
                finished = false;
              }
            }
          }
        }
      }
    }
  };

  while (!finished) {
    finished = true;
    calc_recursive("S", std::vector<std::string>(0));
    for (const auto &production : augmented_grammar) {
      if (production.GetParent() != start_symbol) {
        calc_recursive(production.GetParent(), std::vector<std::string>(0));
      }
    }
  }

  for (const auto &production : augmented_grammar) {
    sort(follows[production.GetParent()].begin(), follows[production.GetParent()].end());
  }

  return follows;
}
