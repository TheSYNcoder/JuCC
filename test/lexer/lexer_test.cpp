#include "lexer/lexer.h"

#include "gtest/gtest.h"

TEST(lexer, lexer) {
  std::string filename("../test/lexer/input.txt");

  FILE *fp = fopen(filename.c_str(), "r");
  int token;
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_INT, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_IDENTIFIER, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_PAREN_OPEN, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_PAREN_CLOSE, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_CURLY_OPEN, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_COUT, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_LEFT_SHIFT, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_LITERAL, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_SEMICOLON, token);
  token = jucc::GetToken(fp);
  ASSERT_EQ(jucc::TOK_CURLY_CLOSE, token);
}
