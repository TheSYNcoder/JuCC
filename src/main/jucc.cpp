#include "main/jucc.h"
using std::cout;

std::string jucc::Hello() { return "Hello World.\n"; }

/**
 * Benchmark testing function do not use.
 */
int jucc::Radd(int x, int y) {
  for (int i = 0; i < y; i++) {
    x++;
  }
  return x;
}
