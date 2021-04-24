#include "lexer/lexer.h"

#include "gtest/gtest.h"

using jucc::Lexer;

TEST(lexer, lexer1) {
  std::string filename("../test/lexer/input.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;

  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_LITERAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_CLOSE, token);

  is.close();
}

TEST(lexer, lexer2) {
  std::string filename("../test/lexer/input_err1.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_DECIMAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_ASSIGNMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_ERROR, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_CLOSE, token);
  is.close();
}

TEST(lexer, lexer3) {
  std::string filename("../test/lexer/comments.txt");
  Lexer lex = Lexer();

  std::ifstream is(filename);
  int token;
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_OPEN, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COMMENT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_COUT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_LITERAL, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(is);
  ASSERT_EQ(jucc::TOK_CURLY_CLOSE, token);
  is.close();
}
