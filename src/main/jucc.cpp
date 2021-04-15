#include "main/jucc.h"
using std::cout;

void jucc::Hello() { cout << "Hello World.\n"; }

/**
 * Benchmark testing function do not use.
 */
int jucc::Radd(int x, int y) {
  for (int i = 0; i < y; i++) {
    x++;
  }
  return x;
}
