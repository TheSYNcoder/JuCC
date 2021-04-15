#include "benchmark/benchmark.h"
#include "main/jucc.h"

// NOLINTNEXTLINE
static void BenchmarkRadd(benchmark::State &state) {
  // NOLINTNEXTLINE
  for (auto _ : state) {
    jucc::Radd(0, 1000000);
  }
}

BENCHMARK(BenchmarkRadd);
