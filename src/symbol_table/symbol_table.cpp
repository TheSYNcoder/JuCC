#include "symbol_table/symbol_table.h"

namespace jucc::symbol_table {

bool LinkedList::IsEmpty() { return this->head_ == nullptr; }

Node *LinkedList::CreateNewNode(std::string it_, std::string dt_, int nt_) {
  Node *node = new Node(std::move(it_), std::move(dt_), nt_);
  return node;
}

void LinkedList::AddNewNode(std::string it_, std::string dt_, int nt_) {
  Node *node = LinkedList::CreateNewNode(std::move(it_), std::move(dt_), nt_);
  node->next_ = head_;
  head_ = node;
}

void LinkedList::DeleteStartingNode() {
  if (IsEmpty()) {
    return;
  }
  Node *temp_ref = head_;
  head_ = temp_ref->next_;
  delete temp_ref;
}

Node *LinkedList::GetHead() { return this->head_; }

void SymbolTable::CheckAndAddEntry(Node *node_) {
  // if present check if in common scope
  if (hash_table_.count(node_->identifier_) != 0U) {
    if (hash_table_[node_->identifier_].GetHead()->nesting_level_ == node_->nesting_level_) {
      // error duplicate symbol
      SymbolTable::InsertIntoDuplicateSymbols(node_->identifier_);
    } else {
      // else we add a new node at the scope
      hash_table_[node_->identifier_].AddNewNode(node_->identifier_, node_->data_type_, node_->nesting_level_);
    }
    return;
  }

  // if not present in common scope just insert it in hash table
  hash_table_[node_->identifier_].AddNewNode(node_->identifier_, node_->data_type_, node_->nesting_level_);
}

void SymbolTable::InsertIntoDuplicateSymbols(const std::string &identifier_) {
  duplicate_symbols_.push_back(identifier_);
}

void SymbolTable::RemoveNodesOnScopeEnd(int level_) {
  for (auto &id_nodes : hash_table_) {
    if (id_nodes.second.GetHead()->nesting_level_ == level_) {
      hash_table_[id_nodes.first].DeleteStartingNode();
      if (hash_table_[id_nodes.first].IsEmpty()) {
        // delete the entry
        hash_table_.erase(hash_table_.find(id_nodes.first));
      }
    }
  }
}

Node *SymbolTable::GetLinkedListById(const std::string &id_) { return hash_table_[id_].GetHead(); }

std::vector<std::string> SymbolTable::GetDuplicateSymbols() { return duplicate_symbols_; }

int SymbolTable::CheckOccurrencesOfId(const std::string &id_) { return hash_table_.count(id_); }
}  // namespace jucc::symbol_table