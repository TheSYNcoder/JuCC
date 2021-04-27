#ifndef JUCC_LEXER_LEXER_H
#define JUCC_LEXER_LEXER_H

#include <fstream>
#include <string>
#include <vector>

#include "symbol_table/symbol_table.h"

namespace jucc {
namespace lexer {

enum Token {
  TOK_EOF = -1,
  TOK_IDENTIFIER = -2,
  TOK_DECIMAL = -3,
  TOK_FRACTIONAL = -4,

  // Conditionals
  TOK_IF = -6,
  TOK_ELSE = -7,

  // Data types
  TOK_INT = -8,
  TOK_FLOAT = -9,
  TOK_VOID = -10,
  // error
  TOK_ERROR = -100,

  // punctuation tokens

  TOK_SEMICOLON = -11,     // ;
  TOK_CURLY_OPEN = -12,    // {
  TOK_CURLY_CLOSE = -13,   // }
  TOK_PAREN_OPEN = -14,    // (
  TOK_PAREN_CLOSE = -15,   // )
  TOK_DOT = -16,           // .
  TOK_COMMA = -17,         // ,
  TOK_LEFT_SHIFT = -18,    // <<
  TOK_RIGHT_SHIFT = -19,   // >>
  TOK_LESS_THAN = -20,     // <
  TOK_GREATER_THAN = -21,  // >
  TOK_EQUAL_TO = -22,      // ==
  TOK_ASSIGNMENT = -23,    // =
  TOK_COMMENT = -24,       //  //
  TOK_LITERAL = -25,       // "c++"
  TOK_CHARACTER = -26,     // 'c'

  // arithmetic tokens

  TOK_PLUS = -29,      // +
  TOK_MINUS = -30,     // -
  TOK_MULTIPLY = -31,  // *
  TOK_DIVIDE = -32,    // /
  TOK_MODULUS = -33,   // %

  // RELATIONAL

  TOK_GREATER_THAN_OR_EQUALS = -34,  // >=
  TOK_LESS_THAN_OR_EQUALS = -35,     // <=
  TOK_NOT = -36,                     // !
  TOK_NOT_EQUAL_TO = -37,            // !=

  // cout, cin

  TOK_COUT = -27,  // cout
  TOK_CIN = -28,   // cin
};

class Lexer {
  /**
   * used to store a identifier token
   */
  std::string identifier_string_;
  /**
   * used to store a corresponding error token
   * suppose a numerical token 16.3ere which is
   * neither a numerical token or a identifier
   */

  std::string error_string_;
  /**
   * used to store a literal string
   * literal strings are of type "a string"
   */

  std::string literal_string_;
  /**
   * used to store the value of the integer token
   * during tokenization.
   */

  int intval_;
  /**
   * used to store the value of the float token
   * during tokenization.
   */

  double floatval_;
  /**
   * The current nesting level as parsed by the lexer in
   * the input file.
   */

  int current_nesting_level_{0};
  /**
   * vector to store duplicate symbol errors.
   */
  std::vector<std::string> duplicate_symbol_errors_;
  /**
   * vector to store undeclared symbol errors.
   */
  std::vector<std::string> undeclared_symbol_errors_;

  /**
   * Stores the current datatype of the identifier.
   * for example int a = 5;
   * When tokenizing a current_datatype_ = "int", for all others it is
   * an empty string.
   */
  std::string current_datatype_;

  /**
   * A symbol table object for building up the symbol table for the input file.
   * Check src/include/symbol_table/symbol_table.h and src/symbol_table/symbol_table.cpp
   * for more details.
   */
  symbol_table::SymbolTable symbol_table_;

 public:
  Lexer() = default;

  /**
   * Takes a ifstream object as input and gets the next character
   * from the input file and returns the appropriate token.
   */
  int GetToken(std::ifstream &is);

  /**
   * Getter for the current_datatype.
   */
  std::string GetCurrentDatatype();

  /**
   * Getter for the current nesting level.
   */
  [[nodiscard]] int GetCurrentNestingLevel() const;

  /**
   * Getter for undeclared symbol errors.
   */
  std::vector<std::string> GetUndeclaredSymbolErrors();

  /**
   * Getter for duplicate symbol errors.
   */
  std::vector<std::string> GetDuplicateSymbolErrors();
};  // class Lexer

}  // namespace lexer
}  // namespace jucc

#endif
