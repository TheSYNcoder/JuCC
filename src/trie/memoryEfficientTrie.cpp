#include "trie/memoryEfficientTrie.h"

namespace jucc {
Trie::Trie() { count_ = 0; }

TrieManager::TrieManager() { master_ = NewTrieNode(); }

void TrieManager::Insert(const grammar::Entity &entities) {
  auto *head = master_;
  grammar::Entity till_keys;
  for (const auto &entity : entities) {
    till_keys.push_back(entity);
    if (head->nodes_.find(entity) == head->nodes_.end()) {
      head->nodes_[entity] = NewTrieNode();
      head->nodes_[entity]->keys_list_ = till_keys;
    }
    head = head->nodes_[entity];
    head->count_++;
  }
}

void TrieManager::InsertAll(const grammar::Production &prod) {
  for (const auto &rule : prod.GetRules()) {
    Insert(rule.GetEntities());
  }
}

void TrieManager::GreedyPreorder(Trie *head, int &len, grammar::Entity &max_str, bool is_prime_head) {
  if (head == nullptr) {
    return;
  }
  bool state_changed = false;
  // Get the node with max count
  if (head->count_ >= len && head->count_ != 1) {
    len = head->count_;
    state_changed = true;
    max_str = head->keys_list_;
  }

  if (state_changed || is_prime_head) {
    for (const auto &node : head->nodes_) {
      GreedyPreorder(node.second, len, max_str, false);
    }
  }
}

Trie *TrieManager::NewTrieNode() {
  Trie *t = new Trie;
  this->gc_.push_back(t);
  return t;
}

TrieManager::~TrieManager() {
  // flushing
  for (auto &node : gc_) {
    delete node;
  }
}

Trie *TrieManager::GetMaster() const { return master_; }

}  // namespace jucc
