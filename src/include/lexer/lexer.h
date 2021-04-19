#ifndef JUCC_LEXER_H
#define JUCC_LEXER_H

#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>

namespace jucc {

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

  // cout, cin

  TOK_COUT = -27,  // cout
  TOK_CIN = -28,   // cin
};

static std::string identifier_string;
static std::string error_string;
static std::string literal_string;
static int intval;
static double floatval;

/**
 * Takes a file pointer as input and gets the next character
 * from the file and returns the appropriate token
 */
int GetToken(FILE *fp);

}  // namespace jucc

#endif
