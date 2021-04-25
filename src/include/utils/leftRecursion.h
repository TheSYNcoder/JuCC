#ifndef JUCC_LEFTRECURSION_H
#define JUCC_LEFTRECURSION_H
#include "grammar/grammar.h"

namespace jucc::utils {
const char DASH[] = "'";

grammar::Productions RemoveDirectLeftRecursion(const grammar::Production & /*prod*/);
bool IsRecursive(const grammar::Production & /*prod*/);

grammar::Productions RemoveIndirectLeftRecursions(const grammar::Productions &);

}  // namespace jucc::utils
#endif  // JUCC_LEFTRECURSION_H
