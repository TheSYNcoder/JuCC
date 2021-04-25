#include "trie/memoryEfficientTrie.h"

jucc::Trie::Trie() { count = 0; }

jucc::TrieManager::TrieManager() { master = NewTrieNode(); }

void jucc::TrieManager::Insert(const grammar::Entity &entities) {
  auto *head = master;
  grammar::Entity tillKeys;
  for (auto &entity : entities) {
    tillKeys.push_back(entity);
    if (head->nodes.find(entity) == head->nodes.end()) {
      head->nodes[entity] = NewTrieNode();
      head->nodes[entity]->keys_list = tillKeys;
    }
    head = head->nodes[entity];
    head->count++;
  }
}

void jucc::TrieManager::InsertAll(const grammar::Production &prod) {
  for (const auto &rule : prod.getRules()) {
    Insert(rule.getEntities());
  }
}

void jucc::TrieManager::GreedyPreorder(Trie *head, int &len, grammar::Entity &max_str, bool is_prime_head) {
  if (head == nullptr) {
    return;
  }
  bool state_changed = false;
  // Get the node with max count
  if (head->count >= len && head->count != 1) {
    len = head->count;
    state_changed = true;
    max_str = head->keys_list;
  }

  if (state_changed || is_prime_head) {
    for (const auto &node : head->nodes) {
      GreedyPreorder(node.second, len, max_str, false);
    }
  }
}

jucc::Trie *jucc::TrieManager::NewTrieNode() {
  Trie *t = new Trie;
  this->gc.push_back(t);
  return t;
}

jucc::TrieManager::~TrieManager() {
  // flushing
  for (auto &node : gc) {
    delete node;
  }
}
jucc::Trie *jucc::TrieManager::GetMaster() const { return master; }
