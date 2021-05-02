#include "main/jucc.h"

#include <cstring>

#include "gtest/gtest.h"
using jucc::InputParser;

TEST(jucc, dummy) { ASSERT_TRUE(true); }

TEST(jucc, Radd) { ASSERT_EQ(100, jucc::Radd(0, 100)); }

TEST(jucc, Parser) {
  int argc = 3;
  char *argv[3];
  argv[0] = strdup(std::string("-h").c_str());
  argv[1] = strdup(std::string("-f").c_str());
  argv[2] = strdup(std::string("filename").c_str());

  auto *parser = new InputParser(argc, argv);
  free(argv[0]);
  free(argv[1]);
  free(argv[2]);
  ASSERT_TRUE(parser->HasFlag("-h"));
  ASSERT_EQ(parser->GetArgument("-f"), "filename");
  ASSERT_FALSE(parser->HasFlag("-t"));
  delete parser;
}

TEST(jucc, hello) { ASSERT_EQ(jucc::Hello().substr(0, 6), "\x1B[1;33"); }
