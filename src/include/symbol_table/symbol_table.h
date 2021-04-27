#ifndef JUCC_SYMBOL_TABLE_SYMBOL_TABLE_H
#define JUCC_SYMBOL_TABLE_SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace jucc {

namespace symbol_table {

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
   *      if ( condition ) {
   *          int a1_;
   *          if ( second_condition ) {
   *              int a2_;
   *              if ( third_condition) {
   *                      int a3_;
   *                     ...
   *              }
   *          }
   *      }
   *      ...
   *      a0_ has 0 level , a1_ has 1 level and so on ...
   */
  int nesting_level_;

  /**
   * The pointer to the next node.
   */
  Node *next_;

  /**
   * Constructor for node class
   */
  Node(std::string it_, std::string dt_, int nt_)
      : identifier_(std::move(it_)), data_type_(std::move(dt_)), nesting_level_(nt_), next_(nullptr) {}
};

class LinkedList {
  /**
   * The head_ of the linked list
   */
  Node *head_{nullptr};

 public:
  LinkedList() = default;

  /**
   * Allocates memory for a new Node and returns it after initializing.
   */
  static Node *CreateNewNode(std::string it_, std::string dt_, int nt_);

  /**
   * Adds a new node at the starting of the linked list
   */
  void AddNewNode(std::string it_, std::string dt_, int nt_);

  /**
   * Deletes the first node of the linked list
   */
  void DeleteStartingNode();

  /**
   * Returns true if linked list is empty or vice-versa
   */
  bool IsEmpty();

  /**
   * returns head_
   */
  Node *GetHead();
};

class SymbolTable {
  /**
   * Store the identfier mappings with respect to their presence
   * in different nesting levels in the program
   */
  std::unordered_map<std::string, LinkedList> hash_table_;

  /**
   * A vector to store different duplicate symbols found in the input
   */
  std::vector<std::string> duplicate_symbols_;

  /**
   * A vector to store the undeclared symbols in the input file.
   */
  std::vector<std::string> undeclared_symbols_;

 public:
  /**
   * Checks if the current identifier is present in the same nesting level
   * int the hash_table. If present reports a duplicate symbol error, that is,
   * inserts into the duplicate_symbols vector.
   */
  void CheckAndAddEntry(Node *node_);

  /**
   * On scope end - sc_
   * Removes the nodes of all the variables in the hash_table which have nesting_level_ = sc_
   */
  void RemoveNodesOnScopeEnd(int level_);

  /**
   * Inserts symbols into duplicate symbols array
   */
  void InsertIntoDuplicateSymbols(const std::string &identifier_);

  /**
   * Returns the linked list or list of nodes associated with an
   *  identifier.
   */
  Node *GetLinkedListById(const std::string &id_);

  /**
   * Getter method for duplicated symbols.
   */
  std::vector<std::string> GetDuplicateSymbols();

  /**
   * Getter method for undeclared variables.
   */
  std::vector<std::string> GetUndeclaredSymbols();

  /**
   * Checks if the identifier is present in hash_table_
   * Utility function for testing
   */
  int CheckOccurrencesOfId(const std::string &id_);
};

}  // namespace symbol_table

}  // namespace jucc

#endif
