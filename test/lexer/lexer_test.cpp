#include "lexer/lexer.h"

#include "gtest/gtest.h"

using jucc::lexer::Lexer;

TEST(lexer, Lexer1) {
  std::string filename("../test/lexer/input.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;

  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMA, token);
  ASSERT_EQ(",", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CIN, token);
  ASSERT_EQ("cin", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_RIGHT_SHIFT, token);
  ASSERT_EQ(">>", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);

  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_FLOAT, token);
  ASSERT_EQ("float", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_FRACTIONAL, token);
  ASSERT_EQ("float_constant", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LITERAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer2) {
  std::string filename("../test/lexer/input_err1.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ERROR, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_RIGHT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LESS_THAN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_FRACTIONAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer3) {
  std::string filename("../test/lexer/comments.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LITERAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer4) {
  std::string filename("../test/lexer/input_err2.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EQUAL_TO, token);
  ASSERT_EQ("==", lex.GetTokenType(token));
  token = lex.GetToken(is);
  // Two errors first one due to 'bruh second one due to ' after h and nothing follows
  ASSERT_EQ(jucc::lexer::TOK_ERROR, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ERROR, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer5) {
  /**
   *  To test the symbol table implementation with lexer
   *
   */
  std::string filename("../test/lexer/scope_error.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  ASSERT_EQ(lex.GetCurrentDatatype(), "int");
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);

  // assert nesting level
  ASSERT_EQ(lex.GetCurrentNestingLevel(), 1);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  ASSERT_EQ(lex.GetDuplicateSymbolErrors().size(), 0);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  ASSERT_EQ(lex.GetDuplicateSymbolErrors().size(), 1);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  ASSERT_EQ(lex.GetUndeclaredSymbolErrors().size(), 0);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  ASSERT_EQ(lex.GetUndeclaredSymbolErrors().size(), 1);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  ASSERT_EQ(lex.GetUndeclaredSymbolErrors().size(), 2);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer6) {
  std::string filename("../test/lexer/arithmetic.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_GREATER_THAN, token);
  ASSERT_EQ(">", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ELSE, token);
  ASSERT_EQ("else", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  ASSERT_EQ("if", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_NOT_EQUAL_TO, token);
  ASSERT_EQ("!=", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MULTIPLY, token);
  ASSERT_EQ("*", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PLUS, token);
  ASSERT_EQ("+", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MINUS, token);
  ASSERT_EQ("-", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_ASSIGNMENT, token);
  ASSERT_EQ("=", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DIVIDE, token);
  ASSERT_EQ("/", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IF, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_GREATER_THAN_OR_EQUALS, token);
  ASSERT_EQ(">=", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_DECIMAL, token);
  ASSERT_EQ("integer_constant", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_IDENTIFIER, token);
  ASSERT_EQ("identifier", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}

TEST(lexer, Lexer7) {
  std::string filename("../test/lexer/comments.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_INT, token);
  ASSERT_EQ("int", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_MAIN, token);
  ASSERT_EQ("main", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_OPEN, token);
  ASSERT_EQ("(", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_PAREN_CLOSE, token);
  ASSERT_EQ(")", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_OPEN, token);
  ASSERT_EQ("{", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_COUT, token);
  ASSERT_EQ("cout", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LEFT_SHIFT, token);
  ASSERT_EQ("<<", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_LITERAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_SEMICOLON, token);
  ASSERT_EQ(";", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_CURLY_CLOSE, token);
  ASSERT_EQ("}", lex.GetTokenType(token));
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::lexer::TOK_EOF, token);
  is.close();
}
