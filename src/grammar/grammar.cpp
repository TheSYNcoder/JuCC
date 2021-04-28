#include "grammar/grammar.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace jucc::grammar {

bool HasParent(const grammar::Productions &productions, const std::string &parent) {
  return std::any_of(productions.begin(), productions.end(),
                     [&](const grammar::Production &prod) { return prod.GetParent() == parent; });
}

grammar::Rules GetRulesForParent(const grammar::Productions &productions, const std::string &parent) {
  for (const auto &production : productions) {
    if (production.GetParent() == parent) {
      return production.GetRules();
    }
  }
  return grammar::Rules();
}

Parser::Parser(const char *filepath) { file_ = std::ifstream(filepath); }

Parser::~Parser() {
  if (file_.is_open()) {
    file_.close();
  }
}

std::vector<std::string> Parser::FastTokenize(const std::string &s) {
  std::vector<std::string> res;
  std::stringstream ss(s);
  std::string token;
  while (ss >> token) {
    res.push_back(token);
  }
  return res;
}

/**
 * This is based on a basic state machine that implicitly uses a grammar to parse.
 * The parse states represent a block of a .g grammar file.
 * Eg. ParseState TERMINALS imples we have seen a %terminals token and yet to see
 * a % block closing token.
 * RuleState keeps track of additonal states required to parse a rule inside of a
 * %rule block.
 */
bool Parser::Parse() {
  // Parser State
  enum ParseState { BASIC, TERMINALS, NON_TERMINALS, START, RULES };

  // Parser State while parsing productions
  // only valid when Parser State is RULES
  enum RuleState { LEFT, COLON, ENTITY };

  const std::string terminals_start = "%terminals";
  const std::string non_terminals_start = "%non_terminals";
  const std::string start_start = "%start";
  const std::string rules_start = "%rules";
  const std::string state_reset_token = "%";

  // check if file is open else return false
  if (file_.is_open()) {
    ParseState curr_parse_state = BASIC;
    std::string line;

    // ugh...
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;

    while (getline(file_, line)) {
      // states for rule parsing
      RuleState curr_rule_state = LEFT;
      std::string production_parent;
      std::vector<std::string> rule_entities;

      // ignore empty lines and comments
      if (line.length() == 0 || line[0] == '#') {
        continue;
      }

      // tokenize line
      std::vector<std::string> tokens = FastTokenize(line);

      // update parser state
      if (tokens[0] == terminals_start) {
        if (curr_parse_state != BASIC || tokens.size() > 1) {
          error_ = "grammar parsing error: invalid token %terminals";
          return false;
        }
        curr_parse_state = TERMINALS;
        tokens.erase(tokens.begin());
      } else if (tokens[0] == non_terminals_start) {
        if (curr_parse_state != BASIC || tokens.size() > 1) {
          error_ = "grammar parsing error: invalid token %non_terminals";
          return false;
        }
        curr_parse_state = NON_TERMINALS;
        tokens.erase(tokens.begin());
      } else if (tokens[0] == start_start) {
        if (curr_parse_state != BASIC || tokens.size() > 1) {
          error_ = "grammar parsing error: invalid token %start";
          return false;
        }
        curr_parse_state = START;
        tokens.erase(tokens.begin());
      } else if (tokens[0] == rules_start) {
        if (curr_parse_state != BASIC || tokens.size() > 1) {
          error_ = "grammar parsing error: invalid token %rules";
          return false;
        }
        curr_parse_state = RULES;
        tokens.erase(tokens.begin());
      } else if (tokens[0] == state_reset_token) {
        if (curr_parse_state == BASIC || tokens.size() > 1) {
          error_ = "grammar parsing error: invalid token %";
          return false;
        }
        curr_parse_state = BASIC;
        tokens.erase(tokens.begin());
      }

      // iterate over tokens
      // add tokens to different lists as a function of Parser State
      // for Parser State RULES parse the production following implicit grammar:
      // "LEFT : ENTITY_LIST"
      // "ENTITY_LIST: ENTITY ENTITY_LIST | ENTITY"
      for (const auto &token : tokens) {
        switch (curr_parse_state) {
          case BASIC:
            error_ = "grammar parsing error: invalid token outside block: " + token;
            return false;
            break;

          case TERMINALS:
            if (token == std::string(EPSILON)) {
              error_ = "grammar parsing error: EPSILON is reserved";
              return false;
            }
            terminals_.push_back(token);
            break;

          case NON_TERMINALS:
            if (token == std::string(EPSILON)) {
              error_ = "grammar parsing error: EPSILON is reserved";
              return false;
            }
            non_terminals_.push_back(token);
            break;

          case START:
            if (!start_symbol_.empty() || token == std::string(EPSILON)) {
              error_ = "grammar parsing error: ambiguous start symbol";
              return false;
            }
            start_symbol_ = token;
            break;

          case RULES:
            switch (curr_rule_state) {
              case LEFT:
                if (token == std::string(EPSILON)) {
                  error_ = "grammar parsing error: production cannot start with EPSILON";
                  return false;
                }
                production_parent = token;
                curr_rule_state = COLON;
                break;

              case COLON:
                if (token != ":") {
                  error_ = "grammar parsing error: rules syntax error ':' expected: " + token;
                  return false;
                }
                curr_rule_state = ENTITY;
                break;

              case ENTITY:
                rule_entities.push_back(token);
                break;

              default:
                break;
            }
          default:
            break;
        }
      }

      if (curr_parse_state == RULES) {
        if (curr_rule_state == ENTITY) {
          grammar[production_parent].push_back(rule_entities);
        }
        if (curr_rule_state == COLON) {
          error_ = "grammar parsing error: rules syntax error ':' expected";
          return false;
        }
      }
    }

    // sanity checks
    // check for undefined tokens and duplicates
    if (curr_parse_state != BASIC) {
      error_ = "grammar parsing error: block is incomplete '%' expected";
      return false;
    }

    std::unordered_set<std::string> terminals;
    std::unordered_set<std::string> non_terminals;

    for (const auto &terminal : terminals_) {
      terminals.insert(terminal);
    }

    if (terminals.size() != terminals_.size()) {
      error_ = "grammar parsing error: inconsistent or duplicate terminals";
      return false;
    }

    for (const auto &nterminal : non_terminals_) {
      if (terminals.find(nterminal) != terminals.end()) {
        error_ = "grammar parsing error: terminals and non_terminals not disjoint";
        return false;
      }
      non_terminals.insert(nterminal);
    }

    if (non_terminals.size() != non_terminals_.size()) {
      error_ = "grammar parsing error: inconsistent or duplicate non_terminals";
      return false;
    }

    // convert std::unordered_map to Production object with checks
    for (const auto &production : grammar) {
      Production prod;
      if (non_terminals.find(production.first) == non_terminals.end()) {
        error_ = "grammar parsing error: non_terminal not found: " + production.first;
        return false;
      }
      prod.SetParent(production.first);

      std::vector<Rule> rules;
      for (const auto &rule : production.second) {
        Rule prod_rule;
        for (const auto &entity : rule) {
          if (non_terminals.find(entity) == non_terminals.end() && terminals.find(entity) == terminals.end() &&
              entity != std::string(EPSILON)) {
            error_ = "grammar parsing error: rule token is not defined: " + entity;
            return false;
          }
        }
        prod_rule.SetEntities(rule);
        rules.push_back(prod_rule);
      }

      prod.SetRules(rules);

      // add Production object created to list of valid productions
      grammar_.push_back(prod);
    }

    return true;
  }

  error_ = "grammar parsing error: file not found";
  return false;
}

std::string Rule::ToString() const {
  std::stringstream ss;
  for (const auto &entity : entities_) {
    ss << entity;
  }
  return ss.str();
}

bool Rule::HasPrefix(const Rule &prefix) const {
  // Takes care of even EPSILON too.
  if (prefix.GetEntities().size() > entities_.size()) {
    return false;
  }

  for (int i = 0; i < static_cast<int>(prefix.GetEntities().size()); i++) {
    if (entities_[i] != prefix.GetEntities()[i]) {
      return false;
    }
  }

  return true;
}

}  // namespace jucc::grammar
