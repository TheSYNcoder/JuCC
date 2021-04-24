#ifndef JUCC_GRAMMAR_H
#define JUCC_GRAMMAR_H
#include <vector>
#include <string>

namespace jucc {
namespace grammar {
const char EPSILON[] = "epsilon";

class Rule {
  std::vector<std::string> entities;
};

class Production {
  std::string parent;
  std::vector<Rule> rules;
};

}  // namespace grammar
}  // namespace jucc
#endif  // JUCC_GRAMMAR_H
