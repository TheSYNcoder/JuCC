#ifndef JUCC_MEMORYEFFICIENTTRIE_H
#define JUCC_MEMORYEFFICIENTTRIE_H
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "grammar/grammar.h"

namespace jucc {

class Trie {
 public:
  // std::vector<Trie *> children;
  std::unordered_map<std::string, Trie *> nodes;
  grammar::Entity keys_list;
  int count;
  Trie();
};

class TrieManager {
  Trie *master;

  // garbage collector
  std::vector<Trie *> gc;

 public:
  TrieManager();
  [[nodiscard]] Trie *GetMaster() const;

  void Insert(const grammar::Entity &);

  void InsertAll(const grammar::Production &);

  static void GreedyPreorder(Trie *, int &, grammar::Entity &, bool);

  Trie *NewTrieNode();

  virtual ~TrieManager();
};

}  // namespace jucc
#endif  // JUCC_MEMORYEFFICIENTTRIE_H
