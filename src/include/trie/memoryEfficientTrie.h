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
  std::unordered_map<std::string, Trie *> nodes_;
  grammar::Entity keys_list_;
  int count_;

  Trie();
};

class TrieManager {
  Trie *master_;

  // garbage collector
  std::vector<Trie *> gc_;

 public:
  TrieManager();
  [[nodiscard]] Trie *GetMaster() const;

  void Insert(const grammar::Entity & /*entities*/);

  void InsertAll(const grammar::Production & /*prod*/);

  // NOLINTNEXTLINE
  static void GreedyPreorder(Trie * /*head*/, int & /*len*/, grammar::Entity & /*max_str*/, bool /*is_prime_head*/);

  Trie *NewTrieNode();

  virtual ~TrieManager();
};

}  // namespace jucc
#endif  // JUCC_MEMORYEFFICIENTTRIE_H
