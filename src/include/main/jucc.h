#ifndef JUCC_JUCC_H
#define JUCC_JUCC_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "grammar/grammar.h"
#include "lexer/lexer.h"
#include "parsing/parsing.h"
#include "parsing/utils/parsing_table.h"
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
  InputParser(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
      tokens_.emplace_back(std::string(argv[i]));
    }
  }

  bool HasFlag(const std::string &flag);

  std::string GetArgument(const std::string &flag);
};

/**
 * Dummy function increments computes x + y the retarded way
 * for benchmarking only
 */
int Radd(int x, int y);
}  // namespace jucc

#endif  // JUCC_JUCC_H
