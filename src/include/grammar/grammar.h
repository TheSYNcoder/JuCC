#ifndef JUCC_GRAMMAR_H
#define JUCC_GRAMMAR_H

#include <string>
#include <vector>

namespace jucc {
namespace grammar {
const char *EPSILON = "epsilon";

class Rule {
  std::vector<std::string> entities;

 public:
  Rule() = default;
  explicit Rule(std::vector<std::string> entities) : entities(std::move(entities)) {}
  [[nodiscard]] const std::vector<std::string> &getEntities() const { return entities; }
  void setEntities(const std::vector<std::string> &entities) { Rule::entities = entities; }
};

class Production {
  std::string parent;
  std::vector<Rule> rules;

 public:
  Production() = default;
  [[nodiscard]] const std::string &getParent() const { return parent; }
  [[nodiscard]] const std::vector<Rule> &getRules() const { return rules; }
  void setParent(const std::string &parent) { Production::parent = parent; }
  void setRules(const std::vector<Rule> &rules) { Production::rules = rules; }
};

typedef std::vector<Production> Productions;

}  // namespace grammar
}  // namespace jucc
#endif  // JUCC_GRAMMAR_H
