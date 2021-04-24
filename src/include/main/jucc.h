#ifndef JUCC_JUCC_H
#define JUCC_JUCC_H

#include <iostream>
#include <string>

namespace jucc {
/**
 * Dummy function to test setup clang and build config
 */
std::string Hello();

/**
 * Dummy function increments computes x + y the retarded way
 * for benchmarking only
 */
int Radd(int x, int y);
}  // namespace jucc

#endif  // JUCC_JUCC_H
