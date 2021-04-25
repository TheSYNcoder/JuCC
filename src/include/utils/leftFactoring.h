#ifndef JUCC_LEFTFACTORING_H
#define JUCC_LEFTFACTORING_H
#include "grammar/grammar.h"
namespace grammar = jucc::grammar;

namespace jucc::utils {
grammar::Productions RemoveLeftFactors(const grammar::Production &);

grammar::Entity LongestCommonPrefix(const grammar::Production &prod);

}  // namespace jucc::utils

#endif  // JUCC_LEFTFACTORING_H
