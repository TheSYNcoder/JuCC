

#include "symbol_table/symbol_table.h"

#include "gtest/gtest.h"

using jucc::symbol_table::LinkedList;
using jucc::symbol_table::Node;
using jucc::symbol_table::SymbolTable;

TEST(symbolTable, test1) {
  /**
   * Checks an empty linked list
   */
  LinkedList list = LinkedList();
  ASSERT_EQ(true, list.IsEmpty());
}

TEST(symbolTable, test2) {
  /**
   * Checks adding of a node
   */
  LinkedList list = LinkedList();
  list.AddNewNode(std::string("x"), std::string("int"), 0);
  ASSERT_EQ(false, list.IsEmpty());
  ASSERT_EQ(std::string("x"), list.GetHead()->identifier_);
  ASSERT_EQ(std::string("int"), list.GetHead()->data_type_);
  ASSERT_EQ(0, list.GetHead()->nesting_level_);
}

TEST(symbolTable, test3) {
  /**
   * Checks the deletion of a node
   */
  LinkedList list = LinkedList();
  list.AddNewNode(std::string("x"), std::string("int"), 0);
  ASSERT_EQ(false, list.IsEmpty());
  list.DeleteStartingNode();
  ASSERT_EQ(true, list.IsEmpty());
}

TEST(symbolTable, test4) {
  /**
   * Check and add entry
   * Check duplicate symbol error
   * Check Deletion
   */
  SymbolTable smb = SymbolTable();
  Node *node = new Node("x", "int", 0);
  smb.CheckAndAddEntry(node);
  delete node;
  Node *head = smb.GetLinkedListById("x");
  ASSERT_EQ(std::string("int"), head->data_type_);
  ASSERT_EQ(0, head->nesting_level_);
  Node *node1 = new Node("x", "int", 1);
  smb.CheckAndAddEntry(node1);
  delete node1;
  head = smb.GetLinkedListById("x");
  ASSERT_EQ(std::string("int"), head->data_type_);
  ASSERT_EQ(1, head->nesting_level_);
  Node *node2 = new Node("x", "int", 1);
  smb.CheckAndAddEntry(node2);
  delete node2;
  std::vector<std::string> dups = smb.GetDuplicateSymbols();
  int sz = static_cast<int>(dups.size());
  ASSERT_EQ(sz, 1);
  ASSERT_EQ(true, "x" == dups[0]);
  smb.RemoveNodesOnScopeEnd(1);
  smb.RemoveNodesOnScopeEnd(0);
  smb.RemoveNodesOnScopeEnd(2);

  ASSERT_EQ(0, smb.CheckOccurrencesOfId("x"));
}
