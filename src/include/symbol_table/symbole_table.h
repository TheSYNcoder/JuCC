#ifndef JUCC_SYMBOL_TABLE_SYMBOL_TABLE_H
#define JUCC_SYMBOL_TABLE_SYMBOL_TABLE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
namespace jucc {

struct Node {
  /**
   * Used to store the name of the identifier
   * obtained during tokenization
   */
  std::string identifier_;
  /**
   * Used to store the data type of the identifier
   * One of int or float
   */
  std::string data_type_;
  /**
   * Used to store the nesting level of scoping
   * Deeper the nesting, higher the scope
   *
   *      ...
   *      int a0_;
   *      if ( condition ){
   *          int a1_;
   *          if ( second_condition ){
   *              int a2_;
   *              if ( third_condition){
   *                      int a3_;
   *                     ...
   *              }
   *          }
   *      }
   *      ...
   *      a0_ has 0 level , a1_ has 1 level and so on ...
   */
  std::string nesting_level_;
  /**
   * Constructir for node class
   */
  Node(std::string it_, std::string dt_, std::string nt_) : identifier_(it_), data_type_(dt_), nesting_level_(nt_) {}
};

typedef struct Node Node;

class Linked_list {
 public:
  /**
   * The head of the linked list
   */
  Node *head_;
  /**
   * Adds a new node at the starting of the linked list
   */
  void AddNewNode(Node *node_);
  /**
   * Deletes the first node of the linked list
   */
  void DeleteStartingNode();
  /**
   * Returns true if linked list is empty or vice-versa
   */
  bool isEmpty();
};

class Symbol_table {
 public:
  /**
   * Store the identfier mappings with respect to their presence
   * in different nesting levels in the program
   */
  std::map<std::string, Linked_list> hash_table_;
  /**
   * A vector to store different duplicate symbols found in the input
   */
  std::vector<std::string> duplicate_symbols;
  /**
   * Checks if the current identifier is present in the same nesting level
   * int the hash_table. If present reports a duplicate symbol error, that is,
   * inserts into the duplicate_symbols vector.
   */
  void CheckAndAddEntry(Node *node_);

  /**
   * On scope end  - sc_
   * Removes the nodes of all the variables in the hash_table which have nesting_level_ = sc_
   */
  void RemoveNodesOnScopeEnd();

  /**
   * Inserts symbols into duplicate symbols array
   */
  void InsertIntoDuplicateSymbols(std::string identifier_);
};

}  // namespace jucc

#endif
