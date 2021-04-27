#ifndef JUCC_FIRST_FOLLOW_H
#define JUCC_FIRST_FOLLOW_H

#include <string>
#include <unordered_map>
#include <vector>

#include "grammar/grammar.h"

namespace jucc::utils {
// grammar::Productions getModifiedGrammar(const grammar::Productions &);

/**
 * For each terminal and non terminal in the grammar compute if symbol is nullable
 * Terminals except EPSILON are defaulted to non nullable
 * @returns an unordered_map keyed by each symbol in the grammar of booleans
 * true if symbol is nullable, false otherwise
 */
std::unordered_map<std::string, bool> CalcNullables(const grammar::Productions & /*augmented_grammar*/);

/**
 * For each non terminal in given set of productions computes Firsts
 * @returns an unordered_map keyed by non terminals in the grammar of a vector of terminals
 */
std::unordered_map<std::string, std::vector<std::string>> CalcFirsts(
    const grammar::Productions & /*augmented_grammar*/, const std::unordered_map<std::string, bool> & /*nullables*/);

/**
 * For each non terminal in given set of productions computes Follows
 * @returns an unordered_map keyed by non terminals in the grammar of a vector of terminals
 */
std::unordered_map<std::string, std::vector<std::string>> CalcFollows(
    const grammar::Productions & /*augmented_grammar*/, const std::unordered_map<std::string, bool> & /*nullables*/,
    const std::string & /*start_symbol*/);

}  // namespace jucc::utils

#endif  // JUCC_FIRST_FOLLOW_H
