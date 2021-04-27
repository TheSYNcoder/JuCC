#include "utils/first_follow.h"
#include <functional>
#include "utils/left_factoring.h"
#include "utils/left_recursion.h"
using namespace std;
bool searchParent(const grammar::Productions &productions, const std::string &parent)
{
    for (auto production : productions) {
        if (production.GetParent() == parent) {
            return true;
        }
    }
    return false;
}

std::vector< std::vector<std::string> > getRulesforParent(const grammar::Productions &productions, const std::string &parent)
{
    std::vector< std::vector<std::string> > rules;
    for (auto production : productions) {
        if (production.GetParent() == parent) {
            for (auto rule : production.GetRules()) {
                rules.push_back(rule.GetEntities());
            }
            break;
        }
    }
    return rules;
}

grammar::Productions getModifiedGrammar(const grammar:: Productions &prods)
{
    grammar::Productions cache;
    for (const auto& prod : prods) {
        for(const auto& nag : jucc::utils::RemoveDirectLeftRecursion(prod))
        {
            auto micro_nag = jucc::utils::RemoveLeftFactors(nag);
            cache.insert(cache.end(), micro_nag.begin(), micro_nag.end());
        }
    }
    return cache;
}

std::unordered_map< std::string, bool > calcNullables(const grammar::Productions &augmentedGrammar)
{
    std::unordered_map< std::string, bool > nullables;
    for (const auto& production : augmentedGrammar) {
        for (const auto& body : production.GetRules()) {
            for (const auto& symbol : body.GetEntities()) {
                if (!searchParent(augmentedGrammar, symbol)) {
                    nullables[symbol] = false;
                }
            }
        }
    }
    if (nullables.find(std::string(grammar::EPSILON)) != nullables.end()) {
        nullables[std::string(grammar::EPSILON)] = true;
    }
    std::function<bool(const std::string&, std::vector<std::string>)> calcRec;
    calcRec = [&] (const std::string& key, std::vector<std::string> path)
    {
        if (find(path.begin(), path.end(), key) != path.end()) {
            return false;
        }
        path.push_back(key);
        if (nullables.find(key) != nullables.end()) {
            return nullables[key];
        }
        for (auto body : getRulesforParent(augmentedGrammar, key)) {
            std::vector<std::string>::iterator symbol;
            for (symbol = body.begin(); symbol != body.end(); symbol++) {
                if (!calcRec(*symbol, path)) {
                    break;
                }
            }
            if (symbol == body.end()) {
                nullables[key] = true;
                return true;
            }
        }
        nullables[key] = false;
        return false;
    };
    for (const auto& production : augmentedGrammar)
    {
        calcRec(production.GetParent(), std::vector<std::string> (0));
    }
    return nullables;
}

std::unordered_map< std::string, std::vector<std::string> > calcFirsts(const grammar::Productions &augmentedGrammar, std::unordered_map< std::string, bool> &nullables)
{
    std::unordered_map< std::string, std::vector<std::string> > firsts;
    bool finished = false;
    for (const auto& production : augmentedGrammar) {
        for (const auto& body : production.GetRules()) {
            for (const auto& symbol : body.GetEntities()) {
                if (searchParent(augmentedGrammar, symbol)) {
                    firsts.insert(make_pair(symbol, std::vector<std::string> (1, symbol)));
                }
            }
        }
    }
    std::function<std::vector<std::string>(const std::string&, std::vector<std::string>)> calcRec;
    calcRec = [&] (const std::string& key, std::vector<std::string> path)
    {
        if (!searchParent(augmentedGrammar, key)) {
            return firsts[key];
        }
        if (find(path.begin(), path.end(), key) != path.end()) {
            return firsts[key];
        }
        path.push_back(key);
        if (firsts.find(key) == firsts.end()) {
            firsts.insert(make_pair(key, std::vector<std::string> (0)));
            finished = false;
        }
        for (auto body : getRulesforParent(augmentedGrammar, key)) {
            std::vector<std::string>::iterator symbol;
            for (symbol = body.begin(); symbol != body.end(); symbol++) {
                std::vector<std::string> first = calcRec(*symbol, path);
                for (const auto& der : first) {
                    if (der != "E" && find(firsts[key].begin(), firsts[key].end(), der) == firsts[key].end()) {
                        firsts[key].push_back(der);
                        finished = false;
                    }
                }
                if (!nullables[*symbol]) {
                    break;
                }
            }
            if (symbol == body.end() && find(firsts[key].begin(), firsts[key].end(), std::string(grammar::EPSILON)) == firsts[key].end()) {
                firsts[key].push_back(std::string(grammar::EPSILON));
            }
        }
        return firsts[key];
    };
    while  (!finished) {
        finished = true;
        for (const auto& production : augmentedGrammar)
        {
            calcRec(production.GetParent(), std::vector<std::string> (0));
        }
    }
    for (const auto& production : augmentedGrammar) {
        sort(firsts[production.GetParent()].begin(), firsts[production.GetParent()].end());
    }
    return firsts;
}

std::unordered_map< string, vector<string> > calcFollows(const grammar::Productions &augmentedGrammar,  std::unordered_map< std::string, bool> nullables, const std::string &start_symbol)
{
    std::unordered_map< string, vector<string> > follows;
    std::unordered_map< std::string, std::vector<std::string> > firsts = calcFirsts(augmentedGrammar, nullables   );
    bool finished = false;
    for (const auto& production : augmentedGrammar) {
        if (production.GetParent() == start_symbol)
            follows.insert(make_pair(production.GetParent(), std::vector<std::string> (1, "$")));
        else
            follows.insert(make_pair(production.GetParent(), std::vector<std::string> (0)));
    }
    std::function<void(const std::string&, std::vector<std::string>)> calcRec;
    calcRec = [&] (const std::string& key, const std::vector<std::string>& path)
    {
        for (auto body : getRulesforParent(augmentedGrammar, key)) {
            std::vector<std::string>::iterator symbol_itr, next_itr;
//            auto ss = *next_itr;
            for (symbol_itr = body.begin(); symbol_itr != body.end(); symbol_itr++) {
                std::string mid = *symbol_itr;
                if (searchParent(augmentedGrammar, key)) {
                    for (next_itr = symbol_itr + 1; next_itr != body.end(); next_itr++) {
                        for (const auto& der : firsts[*next_itr]) {
                            if (der != std::string(grammar::EPSILON) && find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                                follows[mid].push_back(der);
                                finished = false;
                            }
                        }
                        if (!nullables[ *next_itr ]) {
                            break;
                        }
                    }
                    if (next_itr == body.end()) {
                        for (const auto& der : follows[key])
                        {
                            if (find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                                follows[mid].push_back(der);
                                finished = false;
                            }
                        }
                    }
                }
            }
        }
    };
    while  (!finished) {
        finished = true;
        calcRec("S", std::vector<std::string> (0));
        for (const auto& production : augmentedGrammar)
        {
            if (production.GetParent() != start_symbol)
                calcRec(production.GetParent(), std::vector<std::string> (0));
        }
    }
    for (const auto& production : augmentedGrammar) {
        sort(follows[production.GetParent()].begin(), follows[production.GetParent()].end());
    }
    return follows;
}
