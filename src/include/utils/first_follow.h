#ifndef JUCC_FIRST_FOLLOW_H
#define JUCC_FIRST_FOLLOW_H
#include "grammar/grammar.h"
//using namespace std

namespace jucc::utils {
    grammar::Productions getModifiedGrammar(const grammar::Productions &);
    bool searchParent(const grammar::Productions &, const std::string &);
    std::vector< std::vector<std::string> > getRulesforParent(const grammar::Productions &, const std::string &);
    std::unordered_map< std::string, bool > calcNullables(const grammar::Productions &);
    std::unordered_map< std::string, std::vector<std::string>> calcFirsts(const grammar::Productions &,  std::unordered_map< std::string, bool > &);
    std::unordered_map< std::string, std::vector<std::string>> calcFollows(const grammar::Productions &,  std::unordered_map< std::string, bool > &, const std::string &);
}  // namespace jucc::utils

#endif  // JUCC_FIRST_FOLLOW_H
