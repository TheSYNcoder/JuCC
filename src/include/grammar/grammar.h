//
// Created by bisakh on 24/04/21.
//

#ifndef JUCC_GRAMMAR_H
#define JUCC_GRAMMAR_H

namespace jucc {
namespace grammar {
const std::string EPSILON = "epsilon";

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
