#ifndef JUCC_LEFT_RECURSION_H
#define JUCC_LEFT_RECURSION_H
#include "grammar/grammar.h"

namespace jucc::utils {
const char DASH[] = "'";

grammar::Productions RemoveDirectLeftRecursion(const grammar::Production & /*prod*/);
bool IsRecursive(const grammar::Production & /*prod*/);

grammar::Productions RemoveIndirectLeftRecursions(const grammar::Productions &);

}  // namespace jucc::utils
#endif  // JUCC_LEFT_RECURSION_H
