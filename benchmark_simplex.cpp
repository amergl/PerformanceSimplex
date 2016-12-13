//
// Created by fkantere on 12/13/16.
//

#include <benchmark/benchmark.h>
#include "simplex.hxx"


static void BM_FindPivot(benchmark::State& state) {
  int i = 0;
  while(state.KeepRunning()) {
    benchmark::DoNotOptimize(++i);
  }
}

BENCHMARK(BM_FindPivot);

BENCHMARK_MAIN();