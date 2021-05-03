#include "utils/utils.h"

namespace jucc::utils {
grammar::Productions RemoveAllPossibleAmbiguity(const grammar::Productions &prods) {
  grammar::Productions clean;
  for (const auto &prod : prods) {
    for (const grammar::Production &lr_free : RemoveDirectLeftRecursion(prod)) {
      // non ambiguous grammars
      grammar::Productions nag = RemoveLeftFactors(lr_free);
      clean.insert(clean.end(), nag.begin(), nag.end());
    }
  }
  return clean;
}
}  // namespace jucc::utils
