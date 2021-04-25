#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

void get_grammar(map< string, vector< vector<string> > > &grammar)
{
    string head, line, tmp;
    vector<string> body;
    map< string, vector< vector<string> > >::iterator itr;

    ifstream fin;
    fin.open("inputfile.txt");
    while(getline(fin, line))
    {
        stringstream tokens(line);
        tokens >> head;
        tokens >> tmp;
        while(tokens >> tmp)
            body.push_back(tmp);
        itr = grammar.find(head);
        if(itr != grammar.end())
            itr->second.push_back(body);
        else
        {
            grammar.insert(make_pair(head, vector< vector<string> > (1, body)));
        }
        body.clear();
    }
    fin.close();
}

void calcNullables(map< string, vector< vector<string> > > &grammar, map< string, bool > &nullables)
{
    for(const auto& production : grammar) {
        for(const auto& body : production.second) {
            for(const auto& symbol : body) {
                if(grammar.find(symbol) == grammar.end()) {
                    nullables[symbol] = false;
                }
            }
        }
    }
    if(nullables.find("E") != nullables.end()) {
        nullables["E"] = true;
    }
    function<bool(const string&, vector<string>)> calcRec;
    calcRec = [&] (const string& key, vector<string> path)
    {
        if(find(path.begin(), path.end(), key) != path.end()) {
            return false;
        }
        path.push_back(key);
        if(nullables.find(key) != nullables.end()) {
            return nullables[key];
        }
        for(auto body : grammar[key]) {
            vector<string>::iterator symbol;
            for(symbol = body.begin(); symbol != body.end(); symbol++) {
                if(!calcRec(*symbol, path)) {
                    break;
                }
            }
            if(symbol == body.end()) {
                nullables[key] = true;
                return true;
            }
        }
        nullables[key] = false;
        return false;
    };
    for(const auto& production : grammar)
    {
        calcRec(production.first, vector<string> (0));
    }
}

void calcFirsts(map< string, vector< vector<string> > > &grammar, map< string, vector<string> > &firsts)
{
    map< string, bool> nullables;
    calcNullables(grammar, nullables);
    bool finished = false;
    for(const auto& production : grammar) {
        for(const auto& body : production.second) {
            for(const auto& symbol : body) {
                if(grammar.find(symbol) == grammar.end()) {
                    firsts.insert(make_pair(symbol, vector<string> (1, symbol)));
                }
            }
        }
    }
    function<vector<string>(const string&, vector<string>)> calcRec;
    calcRec = [&] (const string& key, vector<string> path)
    {
        if(grammar.find(key) == grammar.end()) {
            return firsts[key];
        }
        if(find(path.begin(), path.end(), key) != path.end()) {
            return firsts[key];
        }
        path.push_back(key);
        if(firsts.find(key) == firsts.end()) {
            firsts.insert(make_pair(key, vector<string> (0)));
            finished = false;
        }
        for(auto body : grammar[key]) {
            vector<string>::iterator symbol;
            for(symbol = body.begin(); symbol != body.end(); symbol++) {
                vector<string> first = calcRec(*symbol, path);
                for(const auto& der : first) {
                    if(find(firsts[key].begin(), firsts[key].end(), der) == firsts[key].end()) {
                        firsts[key].push_back(der);
                        finished = false;
                    }
                }
                if(!nullables[*symbol]) {
                    break;
                }
            }
            if(symbol == body.end() && find(firsts[key].begin(), firsts[key].end(), "E") == firsts[key].end()) {
                firsts[key].push_back("E");
            }
        }
        return firsts[key];
    };
    while (!finished) {
        finished = true;
        for(const auto& production : grammar)
        {
            calcRec(production.first, vector<string> (0));
        }
    }
    for(const auto& production : grammar) {
        sort(firsts[production.first].begin(), firsts[production.first].end());
    }
}

void calcFollows(map< string, vector< vector<string> > > &grammar, map< string, vector<string> > &follows)
{
    map< string, bool> nullables;
    map< string, vector<string> > firsts;
    calcNullables(grammar, nullables);
    calcFirsts(grammar, firsts);
    bool finished = false;
    for(const auto& production : grammar) {
        if(production.first == "S")
            follows.insert(make_pair(production.first, vector<string> (1, "$")));
        else
            follows.insert(make_pair(production.first, vector<string> (0)));
    }
    function<void(const string&, vector<string>)> calcRec;
    calcRec = [&] (const string& key, const vector<string>& path)
    {
        for(auto body : grammar[key]) {
            vector<string>::iterator symbol_itr, next_itr;
            for(symbol_itr = body.begin(); symbol_itr != body.end(); symbol_itr++) {
                string mid = *symbol_itr;
                if(grammar.find(mid) != grammar.end()) {
                    for(next_itr = symbol_itr + 1; next_itr != body.end(); next_itr++) {
                        for(const auto& der : firsts[*next_itr]) {
                            if(der != "E" && find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                                follows[mid].push_back(der);
                                finished = false;
                            }
                        }
                        if(!nullables[*next_itr]) {
                            break;
                        }
                    }
                    if(next_itr == body.end()) {
                        for(const auto& der : follows[key])
                        {
                            if(find(follows[mid].begin(), follows[mid].end(), der) == follows[mid].end()) {
                                follows[mid].push_back(der);
                                finished = false;
                            }
                        }
                    }
                }
            }
        }
    };
    while (!finished) {
        finished = true;
        calcRec("S", vector<string> (0));
        for(const auto& production : grammar)
        {
            if(production.first != "S")
                calcRec(production.first, vector<string> (0));
        }
    }
    for(const auto& production : grammar) {
        sort(firsts[production.first].begin(), firsts[production.first].end());
    }
}

int main()
{
    map< string, vector< vector<string> > > grammar;
    map< string, vector<string> > first, follow;

    get_grammar(grammar);
    calcFollows(grammar, follow);

    for(const auto& x : grammar) {
        cout << x.first << " = ";
        for(const auto& y : follow[x.first]) {
            cout << y << " ";
        }
        cout << "\n";
    }

    return 0;
}
