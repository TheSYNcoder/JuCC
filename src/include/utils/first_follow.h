#ifndef JUCC_FIRST_FOLLOW_H
#define JUCC_FIRST_FOLLOW_H

#include <string>
#include <unordered_map>
#include <vector>

#include "grammar/grammar.h"

namespace jucc::utils {
// Standard practice to mark input end
const char STRING_ENDMARKER[] = "$";

// grammar::Productions getModifiedGrammar(const grammar::Productions &);

/**
 * For each terminal and non terminal in the grammar compute if symbol is / derives to nullable
 * Terminals except EPSILON are defaulted to non nullable
 * @returns an unordered_map keyed by each symbol in the grammar of booleans
 * true if symbol is nullable, false otherwise.
 */
std::unordered_map<std::string, bool> CalcNullables(const grammar::Productions & /*augmented_grammar*/);

using SymbolsMap = std::unordered_map<std::string, std::vector<std::string>>;

/**
 * For each non terminal in given set of productions computes Firsts, and in case of terminals
 * it returns the same.
 * @returns an unordered_map keyed by the each symbol in the grammar of a vector of terminals.
 */
SymbolsMap CalcFirsts(const grammar::Productions & /*augmented_grammar*/,
                      const std::unordered_map<std::string, bool> & /*nullables*/);

/**
 * For each non terminal in given set of productions computes Follows
 * @returns an unordered_map keyed by non terminals in the grammar of a vector of terminals.
 */
SymbolsMap CalcFollows(const grammar::Productions & /*augmented_grammar*/, const SymbolsMap & /*firsts*/,
                       const std::unordered_map<std::string, bool> & /*nullables*/,
                       const std::string & /*start_symbol*/);

}  // namespace jucc::utils

#endif  // JUCC_FIRST_FOLLOW_H
