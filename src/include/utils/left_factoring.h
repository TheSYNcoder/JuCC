#ifndef JUCC_LEFT_FACTORING_H
#define JUCC_LEFT_FACTORING_H
#include "grammar/grammar.h"
namespace grammar = jucc::grammar;

namespace jucc::utils {
/**
 * Does remove left factor from a production.
 * @return a set of productions free from left factors.
 */
grammar::Productions RemoveLeftFactors(const grammar::Production & /*prod*/);

/**
 * Finds longest prefix which is most common to the rules of the productions.
 * @return a grammar Entity which can be used as a core of grammar::Rule.
 */
grammar::Entity LongestCommonPrefix(const grammar::Production & /*prod*/);

}  // namespace jucc::utils

#endif  // JUCC_LEFT_FACTORING_H
