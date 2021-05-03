#ifndef JUCC_UTILS_UTILS_H
#define JUCC_UTILS_UTILS_H

#include "grammar/grammar.h"
#include "utils/first_follow.h"
#include "utils/left_factoring.h"
#include "utils/left_recursion.h"
#include "utils/trie/memory_efficient_trie.h"

namespace jucc {
namespace utils {
/**
 * Makes the grammar non ambiguous.
 * @return A set of production free from left recursions and left factors.
 */
grammar::Productions RemoveAllPossibleAmbiguity(const grammar::Productions & /*prods*/);
}  // namespace utils
}  // namespace jucc
#endif
