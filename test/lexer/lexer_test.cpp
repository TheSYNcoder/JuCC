#include "lexer/lexer.h"

#include "gtest/gtest.h"

using jucc::Lexer;

TEST(lexer, lexer) {
  std::string filename("../test/lexer/input.txt");
  Lexer lex = Lexer();

  FILE *fp = fopen(filename.c_str(), "r");
  int token;
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_PAREN_OPEN, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_PAREN_CLOSE, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_CURLY_OPEN, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_COUT, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_LEFT_SHIFT, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_LITERAL, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = lex.GetToken(fp);
  ASSERT_EQ(jucc::TOK_CURLY_CLOSE, token);
}
