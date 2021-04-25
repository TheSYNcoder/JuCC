#ifndef JUCC_LEFT_RECURSION_H
#define JUCC_LEFT_RECURSION_H
#include "grammar/grammar.h"

namespace jucc::utils {
// Standard practice.
const char DASH[] = "'";

/**
 * Removes  direct left Recursion.
 * Example: for production
 * E -> E + T
 * E -> T
 * E -> EPSILON
 * @return a set of productions after removing left recursion.
 * E'   -> +TE' | EPSILON
 * E    -> TE' | EPSILON E'
 */
grammar::Productions RemoveDirectLeftRecursion(const grammar::Production & /*prod*/);

/**
 * Checks if a given production is recursive.
 * @return boolean
 */
bool IsRecursive(const grammar::Production & /*prod*/);

/**
 * TODO!! (If required)
 * Removes Indirect Productions.
 * A -> B
 * B -> C
 * C -> A | B | EPSILON
 * @return a set of Productions.
 */
grammar::Productions RemoveIndirectLeftRecursions(const grammar::Productions & /*prod*/);

}  // namespace jucc::utils
#endif  // JUCC_LEFT_RECURSION_H
