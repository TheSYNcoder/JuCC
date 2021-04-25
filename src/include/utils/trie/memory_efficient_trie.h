#ifndef JUCC_UTILS_MEMORY_EFFICIENT_TRIE_H
#define JUCC_UTILS_MEMORY_EFFICIENT_TRIE_H
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "grammar/grammar.h"

namespace jucc {
namespace utils {

class Trie {
  /**
   * Class Trie to store a production efficiently for any required
   * operations, mainly for prefix matching.
   */
 public:
  // Creates a trie for set of Rules where each entity is used as a key.
  std::unordered_map<std::string, Trie *> nodes_;

  grammar::Entity keys_list_;  // Stores the prefix list of entities upto the current entity.
  int count_;                  // Number of occurrences of the current entity after insertion of a set of Rules.

  Trie();
};

class TrieManager {
  /**
   * A higher order abstration to manager a complete Trie.
   * Takes care of unwanted memory leaking through a custom garbage collector.
   */
  Trie *master_;  // Current head of the trie.

  std::vector<Trie *> gc_;  // The garbage collector which stores any newly created trie node.

 public:
  /**
   * Constructor.
   */
  TrieManager();

  /**
   * Customised no-param constructor.
   */
  Trie *NewTrieNode();

  /**
   * Getter: Returns master_ node, i.e. the Head of the trie.
   */
  [[nodiscard]] Trie *GetMaster() const;

  /**
   * Insert a particular grammar Entity into the master_.
   * @param grammar::Entity &
   */
  void Insert(const grammar::Entity & /*entities*/);

  /**
   * Insert a whole production into the trie node master_.
   */
  void InsertAll(const grammar::Production & /*prod*/);

  /**
   * Makes a preorder traversal efficiently of the master_ node of the Trie and
   * returns the most common prefix of the Production Rules formed by individual
   * Rule entities.
   */
  // NOLINTNEXTLINE
  static void GreedyPreorder(Trie * /*head*/, int & /*len*/, grammar::Entity & /*max_str*/, bool /*is_prime_head*/);

  /**
   * Virtual Destructor performs garbage collection where created memory gets released.
   */
  virtual ~TrieManager();
};

}  // namespace utils
}  // namespace jucc
#endif  // JUCC_MEMORYEFFICIENTTRIE_H
