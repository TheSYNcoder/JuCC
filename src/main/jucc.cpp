#include "main/jucc.h"

#include <algorithm>

namespace jucc {

std::string Hello() {
  std::string ret;
  ret += "\033[1;33m";
  ret += "    .---.                 _..._          _..._  ";
  ret += "\033[0m\n";
  ret += "\033[1;33m";
  ret += "     |   |              .-'_..._''.    .-'_..._''.  ";
  ret += "\033[0m\n";
  ret += "\033[1;32m";
  ret += "    '---'            .' .'      '.\\ .' .'      '.\\ ";
  ret += "\033[0m\n";
  ret += "\033[1;36m";
  ret += "    .---.           / .'           / .'            ";
  ret += "\033[0m\n";
  ret += "\033[1;34m";
  ret += "    |   |          . '            . '              ";
  ret += "\033[0m\n";
  ret += "\033[1;35m";
  ret += "    |   |          | |            | |              ";
  ret += "\033[0m\n";
  ret += "\033[1;31m";
  ret += "    |   |   _    _ | |            | |              ";
  ret += "\033[0m\n";
  ret += "\033[1;33m";
  ret += "    |   |  | '  / |. '            . '              ";
  ret += "\033[0m\n";
  ret += "\033[1;33m";
  ret += "    |   | .' | .' | \\ '.          .\\ '.          . ";
  ret += "\033[0m\n";
  ret += "\033[1;32m";
  ret += "    |   | /  | /  |  '. `._____.-'/ '. `._____.-'/ ";
  ret += "\033[0m\n";
  ret += "\033[1;36m";
  ret += " __.'   '|   `'.  |    `-.______ /    `-.______ /  ";
  ret += "\033[0m\n";
  ret += "\033[1;36m";
  ret += "|      ' '   .'|  '/            `              `   ";
  ret += "\033[0m\n";
  ret += "\033[1;35m";
  ret += "|____.'   `-'  `--'                                ";
  ret += "\033[0m\n";
  return ret;
}

/**
 * Benchmark testing function do not use.
 */
int Radd(int x, int y) {
  for (int i = 0; i < y; i++) {
    x++;
  }
  return x;
}

InputParser::InputParser(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    tokens_.emplace_back(std::string(argv[i]));
  }
}

bool InputParser::HasFlag(const std::string &flag) {
  return std::find(tokens_.begin(), tokens_.end(), flag) != tokens_.end();
}

std::string InputParser::GetArgument(const std::string &flag) {
  std::vector<std::string>::const_iterator itr;
  itr = std::find(tokens_.begin(), tokens_.end(), flag);
  if (itr != tokens_.end() && ++itr != tokens_.end()) {
    return *itr;
  }
  return std::string("");
}

}  // namespace jucc
