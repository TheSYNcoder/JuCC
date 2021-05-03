#ifndef JUCC_JUCC_H
#define JUCC_JUCC_H

#include <iostream>
#include <string>
#include <vector>

#include "grammar/grammar.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/parsing_table.h"
#include "utils/first_follow.h"
#include "utils/left_factoring.h"
#include "utils/left_recursion.h"

namespace jucc {
/**
 * Dummy function to test setup clang and build config
 */
std::string Hello();

class InputParser {
  std::vector<std::string> tokens_;

 public:
  /**
   * public constructor for initializing command line arguments
   * and converting them to string tokens for efficient search
   * for flags in the command line input
   */
  InputParser(int argc, char *argv[]);

  /**
   * Returns true if the command line options has the required flag 'flag'
   * Searches the tokens
   * @ Returns true or false whether the given input flag is present in the
   * command line options.
   */
  bool HasFlag(const std::string &flag);

  /**
   * Returns the next argument for a flag in command line options.
   * $ jucc -f <filename>
   * GetArgument("-f") returns filename
   * @ Returns string
   */
  std::string GetArgument(const std::string &flag);
};

/**
 * Dummy function increments computes x + y the retarded way
 * for benchmarking only
 */
int Radd(int x, int y);
}  // namespace jucc

#endif  // JUCC_JUCC_H
