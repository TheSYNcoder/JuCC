#include "utils/first_follow.h"

#include <functional>

#include "utils/left_factoring.h"
#include "utils/left_recursion.h"

namespace jucc::utils {
std::unordered_map<std::string, bool> CalcNullables(const grammar::Productions &augmented_grammar) {
  std::unordered_map<std::string, bool> nullables;
  // set all terminals to non - nullable
  for (const auto &production : augmented_grammar) {
    for (const auto &rule : production.GetRules()) {
      for (const auto &symbol : rule.GetEntities()) {
        // if symbol is not present as a parent in grammar, then it is a terminal
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
  std::function<bool(const std::string &, std::vector<std::string> &)> calc_recursive;
  // lambda function that returns boolean value
  calc_recursive = [&](const std::string &key, std::vector<std::string> &path) {
    if (find(path.begin(), path.end(), key) != path.end()) {
      return false;
    }

    // marking current symbol as visited
    path.push_back(key);

    // registering non-terminal in FOLLOW map if encountered for the first time
    if (nullables.find(key) != nullables.end()) {
      path.pop_back();
      return nullables[key];
    }

    for (const auto &rule : GetRulesForParent(augmented_grammar, key)) {
      std::vector<std::string>::const_iterator symbol_itr;
      for (symbol_itr = rule.GetEntities().begin(); symbol_itr != rule.GetEntities().end(); symbol_itr++) {
        // exit loop if non-nullable symbol is encountered
        if (!calc_recursive(*symbol_itr, path)) {
          break;
        }
      }

      // for production A -> X Y Z, A is nullable iff X, Y, Z all are nullable
      if (symbol_itr == rule.GetEntities().end()) {
        nullables[key] = true;
        path.pop_back();
        return true;
      }
    }

    // by default, a symbol is considered non-nullable
    nullables[key] = false;
    path.pop_back();
    return false;
  };

  for (const auto &production : augmented_grammar) {
    auto path = std::vector<std::string>(0);
    calc_recursive(production.GetParent(), path);
  }

  return nullables;
}

std::unordered_map<std::string, std::vector<std::string>> CalcFirsts(
    const grammar::Productions &augmented_grammar, const std::unordered_map<std::string, bool> &nullables) {
  std::unordered_map<std::string, std::vector<std::string>> firsts;
  // finished -> used to check if any new symbols are added to any FIRST(non-terminal) in a particular iteration
  // useful in case of cycles in productions
  bool finished = false;
  for (const auto &production : augmented_grammar) {
    for (const auto &rules : production.GetRules()) {
      for (const auto &symbol : rules.GetEntities()) {
        if (!SearchParent(augmented_grammar, symbol)) {
          // if symbol is not present as a parent in grammar, then it is a terminal
          // if X is a terminal, then FIRST(X) = {X}
          firsts.insert(make_pair(symbol, std::vector<std::string>(1, symbol)));
        }
      }
    }
  }

  /* at this point, map of FIRST only contains terminal -> {terminal} mappings */
  std::function<std::vector<std::string>(const std::string &, std::vector<std::string> &)> calc_recursive;
  // key  -> current symbol being explored
  // path -> list of all the symbols visited uptil reaching key
  // lambda function that returns vector<string>
  calc_recursive = [&](const std::string &key, std::vector<std::string> &path) {
    // FIRST(terminal) = {terminal}
    if (!SearchParent(augmented_grammar, key)) {
      return firsts[key];
    }

    // if key "belongs to" path, then first of key has already been calculated / visited
    if (find(path.begin(), path.end(), key) != path.end()) {
      return firsts[key];
    }

    // marking current symbol as visited
    path.push_back(key);

    /* now only un-visited non-terminals are left to be explored */

    // registering non-terminal in FIRST map if encountered for the first time
    if (firsts.find(key) == firsts.end()) {
      firsts.insert(make_pair(key, std::vector<std::string>(0)));
      finished = false;
    }

    /* if X is a non-terminal and X -> Y1 Y2 Y3 ... Yk is a production, then
       for 1 <= i <= k, FIRST(X) includes FIRST(Yi) iff FIRST(Y1 ... Yi-1) => EPSILON
       and FIRST(Y1 ... Yi-1) => EPSILON iff for all 1 <= j <= i - 1, Yj => EPSILON */
    for (const auto &rules : GetRulesForParent(augmented_grammar, key)) {
      // symbol_itr -> iterates through all symbols in the body of a production
      std::vector<std::string>::const_iterator symbol_itr;
      for (symbol_itr = rules.GetEntities().begin(); symbol_itr != rules.GetEntities().end(); symbol_itr++) {
        // deriving FIRST(current symbol)
        std::vector<std::string> first = calc_recursive(*symbol_itr, path);
        // der -> each terminal or EPSILON in FIRST(current symbol)
        for (const auto &der : first) {
          if (der != std::string(grammar::EPSILON) &&
              find(firsts[key].begin(), firsts[key].end(), der) == firsts[key].end()) {
            // include all terminals except EPSILON
            firsts[key].push_back(der);
            finished = false;
          }
        }

        // non-nullable symbol encountered, so no need of deriving succeeding symbols
        if (!nullables.at(*symbol_itr)) {
          break;
        }
      }

      // if all symbols of the body of a production yielded to EPSILON, then FIRST(key) includes EPSILON
      if (symbol_itr == rules.GetEntities().end() &&
          find(firsts[key].begin(), firsts[key].end(), std::string(grammar::EPSILON)) == firsts[key].end()) {
        firsts[key].push_back(std::string(grammar::EPSILON));
      }
    }

    path.pop_back();
    return firsts[key];
  };

  /*
      Continue finding FIRST of all non-terminals in grammar as long as some changes are committed
      In case of cycles in productions, we may need to iterate multiple times

      For example, A -> C a, B -> C b, C -> B c | EPSILON
      After iteration 1: FIRST(A) = {a}, FIRST(B) = {b}, FIRST(C) = {EPSILON, b}, finished = false
      After iteration 2: FIRST(A) = {a, b}, FIRST(B) = {b}, FIRST(C) = {EPSILON, b}, finished = false
      After iteration 3: no changes -> stop, finished = true
  */
  while (!finished) {
    finished = true;
    for (const auto &production : augmented_grammar) {
      auto path = std::vector<std::string>(0);
      calc_recursive(production.GetParent(), path);
    }
  }

  // sorting FIRST mappings for convenience in presentation
  for (const auto &production : augmented_grammar) {
    sort(firsts[production.GetParent()].begin(), firsts[production.GetParent()].end());
  }

  return firsts;
}

std::unordered_map<std::string, std::vector<std::string>> CalcFollows(
    const grammar::Productions &augmented_grammar, const std::unordered_map<std::string, bool> &nullables,
    const std::string &start_symbol) {
  std::unordered_map<std::string, std::vector<std::string>> follows;

  // calculate nullables and firsts as prerequisites
  std::unordered_map<std::string, std::vector<std::string>> firsts = CalcFirsts(augmented_grammar, nullables);
  bool finished = false;

  for (const auto &production : augmented_grammar) {
    if (production.GetParent() == start_symbol) {
      // if production head / parent is the start_symbol, then FOLLOW(head) = {$}
      // $ -> input endmarker
      follows.insert(make_pair(production.GetParent(), std::vector<std::string>(1, STRING_ENDMARKER)));
    } else {
      follows.insert(make_pair(production.GetParent(), std::vector<std::string>(0)));
    }
  }

  std::function<void(const std::string &)> calc_recursive;
  // lambda function that returns nothing
  calc_recursive = [&](const std::string &key) {
    for (const auto &rule : GetRulesForParent(augmented_grammar, key)) {
      std::vector<std::string>::const_iterator symbol_itr;
      std::vector<std::string>::const_iterator next_itr;
      for (symbol_itr = rule.GetEntities().begin(); symbol_itr != rule.GetEntities().end(); symbol_itr++) {
        std::string mid = *symbol_itr;
        // if A -> alpha B beta is a production, where B -> non-terminal & alpha, beta -> set of symbols
        // then FOLLOW(B) contains {FIRST(beta) - EPSILON}
        // if mid aka current symbol is terminal, then ignore
        if (SearchParent(augmented_grammar, mid)) {
          for (next_itr = symbol_itr + 1; next_itr != rule.GetEntities().end(); next_itr++) {
            // next_itr -> iterates through every symbol in beta
            for (const auto &der : firsts[*next_itr]) {
              // discarding EPSILON
              if (der != std::string(grammar::EPSILON) &&
                  find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                follows[mid].push_back(der);
                finished = false;
              }
            }

            // non-nullable symbol encountered, so no need of deriving succeeding symbols
            if (!nullables.at(*next_itr)) {
              break;
            }
          }

          /* if execution has reached this point, then beta => EPSILON */

          // if A -> alpha B / A -> alpha B beta & beta => EPSILON, then
          // FOLLOW(B) includes FOLLOW(A)
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

  // boolean variable finished used for same purpose as in CalcFirsts
  while (!finished) {
    finished = true;
    calc_recursive(start_symbol);
    for (const auto &production : augmented_grammar) {
      // FOLLOW(start_symbol) remains unchanged, and is always {$}
      if (production.GetParent() != start_symbol) {
        calc_recursive(production.GetParent());
      }
    }
  }

  // sorting FOLLOW mappings for convenience in presentation
  for (const auto &production : augmented_grammar) {
    sort(follows[production.GetParent()].begin(), follows[production.GetParent()].end());
  }

  return follows;
}

}  // namespace jucc::utils
