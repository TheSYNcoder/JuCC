#ifndef JUCC_GRAMMAR_GRAMMAR_H
#define JUCC_GRAMMAR_GRAMMAR_H

#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace jucc {
namespace grammar {
const char EPSILON[] = "EPSILON";

class Rule {
  /**
   * An entity is a single terminal or non terminal in the right hand side
   * of a production.
   * Example:
   * For production: E : F + E => { "F", "+", "E" } is a rule.
   */
  std::vector<std::string> entities_;

 public:
  Rule() = default;
  explicit Rule(std::vector<std::string> entities) : entities_(std::move(entities)) {}

  /* getters and setters*/
  [[nodiscard]] const std::vector<std::string> &GetEntities() const { return entities_; }
  void SetEntities(const std::vector<std::string> &entities) { Rule::entities_ = entities; }
  [[nodiscard]] std::string ToString() const;

  /**
   * Takes an Rule and checks if the entries of the Rule is a perfect
   * prefix of the this->entities_ or not.
   * @return a boolean after checking if param is actually a prefix or not.
   */
  [[nodiscard]] bool HasPrefix(const Rule & /*prefix*/) const;
};

using Rules = std::vector<Rule>;

class Production {
  /**
   * class Parser returns a list of Productions.
   * Example:
   * For productions: E : F + E
   *                  E : EPSILON
   * parent = "E"
   * rules = { Rule1, Rule2 }
   */
  std::string parent_;
  Rules rules_;

 public:
  Production() = default;
  Production(std::string parent, Rules rules) : parent_(std::move(parent)), rules_(std::move(rules)) {}

  /* getters and setters*/
  [[nodiscard]] const std::string &GetParent() const { return parent_; }
  [[nodiscard]] const Rules &GetRules() const { return rules_; }
  void SetParent(const std::string &parent) { Production::parent_ = parent; }
  void SetRules(const Rules &rules) { Production::rules_ = rules; }
};

using Productions = std::vector<Production>;

class FlatProduction {
  /**
   * class FlatProduction is a limited version of class Production
   * Maps a parent to a single Rule
   * Example:
   * For production: E : F + E
   * parent = "E"
   * rule = {"F", "+", "E"}
   */
  std::string parent_;
  Rule rule_;

 public:
  FlatProduction() = default;
  FlatProduction(std::string parent, Rule rule) : parent_(std::move(parent)), rule_(std::move(rule)) {}

  /* getters and setters*/
  [[nodiscard]] const std::string &GetParent() const { return parent_; }
  [[nodiscard]] const Rule &GetRules() const { return rule_; }
  void SetParent(const std::string &parent) { FlatProduction::parent_ = parent; }
  void SetRules(const Rule &rule) { FlatProduction::rule_ = rule; }

  // To create a consistent hashable object to use in unordered_map
  bool operator==(const FlatProduction &fp) const {
    return parent_ == fp.parent_ && rule_.ToString() == fp.rule_.ToString();
  }
};

class FlatProductionHasher {
 public:
  /**
   * We use predfined hash functions of strings
   * and define our hash function as XOR of the
   * hash values.
   */
  size_t operator()(const FlatProduction &fp) const {
    return (std::hash<std::string>()(fp.GetParent())) ^ (std::hash<std::string>()(fp.GetRules().ToString()));
  }
};

using FlatProductions = std::vector<FlatProduction>;

/**
 * Search if a production exists for a given parent
 * utility function
 * @return if parent symbol is present in productions
 */
bool HasParent(const grammar::Productions & /*productions*/, const std::string & /*parent*/);

/**
 * Given a parent finds all rules for it in the set of productions
 * @returns a vector of rules for the given parent
 */
Rules GetRulesForParent(const grammar::Productions & /*productions*/, const std::string & /*parent*/);

class Parser {
  std::ifstream file_;
  std::vector<std::string> terminals_;      // Terminals defined in grammar file
  std::vector<std::string> non_terminals_;  // Non terminals defined in grammar file
  std::string start_symbol_;                // Start symbol for the grammar
  Productions grammar_;                     // Production rules
  std::string error_;                       // parser error message

  /**
   * Splits input string via spaces.
   */
  static std::vector<std::string> FastTokenize(const std::string &s);

 public:
  /**
   * Constructor
   * @param filepath : opens std::ifstream file_
   */
  explicit Parser(const char *filepath);

  /**
   * Destructor
   * closes std::ifstream file_
   */
  ~Parser();

  /**
   * Parses the input file and populates private variables.
   * Returns true if successful.
   * On error returns false and sets error_.
   */
  bool Parse();

  /**
   * Getters for each private variable.
   */
  std::vector<std::string> GetTerminals() { return terminals_; }
  std::vector<std::string> GetNonTerminals() { return non_terminals_; }
  std::string GetStartSymbol() { return start_symbol_; }
  Productions GetProductions() { return grammar_; }
  std::string GetError() { return error_; }
};

}  // namespace grammar
}  // namespace jucc

#endif  // JUCC_GRAMMAR_GRAMMAR_H
