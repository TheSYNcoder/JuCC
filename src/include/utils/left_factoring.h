#ifndef JUCC_LEFT_FACTORING_H
#define JUCC_LEFT_FACTORING_H
#include "grammar/grammar.h"
namespace grammar = jucc::grammar;

namespace jucc::utils {
grammar::Productions RemoveLeftFactors(const grammar::Production & /*prod*/);

grammar::Entity LongestCommonPrefix(const grammar::Production & /*prod*/);

}  // namespace jucc::utils

#endif  // JUCC_LEFT_FACTORING_H
