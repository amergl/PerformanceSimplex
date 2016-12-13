//
// Created by fkantere on 12/13/16.
//

#include <benchmark/benchmark.h>
#include <stdlib.h>
#include <iostream>
#include "simplex.hxx"


static std::vector<double> generateData(size_t m, size_t n) {
  std::vector<double> data;
  for (size_t i = 0; i < m * n; ++i) {
    data.emplace_back(std::rand());
  }
  return data;
}

static void BM_FindPivot(benchmark::State &state, int m, int n) {
  int p_row = 0;
  int p_col = 0;
  auto data = generateData(m, n);
  while (state.KeepRunning()) {
    bool result = findPivot(data.data(), m, n, p_row, p_col);
    benchmark::DoNotOptimize(result);
  }
}

static void BM_eliminate(benchmark::State &state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    int m = 1000;
    int n = 1000;
    int p_row = 0;
    int p_col = 0;
    auto data = generateData(m, n);
    state.ResumeTiming();
    bool result = findPivot(data.data(), m, n, p_row, p_col);
    eliminate(data.data(), m, n, p_row, p_col);
  }
}

template<class StringType>
static void BM_simplex(benchmark::State &state, StringType file) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    double *array = 0;
    double *solution = 0;
    double *calculated = 0;
    int m = 0, n = 0, solution_size = 0;

    if (!read(file, &array, m, n, &solution, solution_size)) {
      state.SkipWithError("Fehler beim Lesen der Daten");
      break;
    }
    state.ResumeTiming();

    simplex(array, m, n, &calculated, solution_size);
    delete[] array;
  }
}

template<class StringType>
static void BM_dualsimplex(benchmark::State &state, StringType file) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    double *array = 0;
    double *solution = 0;
    double *calculated = 0;
    int m = 0, n = 0, solution_size = 0;
    if (!read(file, &array, m, n, &solution, solution_size)) {
      state.SkipWithError("Fehler beim Lesen der Daten");
      break;
    }
    state.ResumeTiming();

    dualsimplex(array, m, n, &calculated, solution_size);
    delete[] array;
  }
}


template<class StringType>
static void BM_read(benchmark::State &state, StringType file) {
  while (state.KeepRunning()) {
    double *array = 0;
    double *solution = 0;
    double *calculated = 0;
    int m = 0, n = 0, solution_size = 0;
    if (!read(file, &array, m, n, &solution, solution_size)) {
      state.SkipWithError("Fehler beim Lesen der Daten");
      break;
    }

    delete[] array;
  }
}



BENCHMARK_CAPTURE(BM_FindPivot, s1000x1000, 1000, 1000);
//BENCHMARK(BM_eliminate); // not working yet

BENCHMARK_CAPTURE(BM_simplex, data1, "test/data.txt");
BENCHMARK_CAPTURE(BM_simplex, data2, "test/data2.txt");
BENCHMARK_CAPTURE(BM_dualsimplex, data1, "test/data.txt");
BENCHMARK_CAPTURE(BM_dualsimplex, data2, "test/data2.txt");
BENCHMARK_CAPTURE(BM_dualsimplex, data3, "test/data3.txt");

BENCHMARK_CAPTURE(BM_read, data1, "test/data.txt");
BENCHMARK_CAPTURE(BM_read, data2, "test/data2.txt");
BENCHMARK_CAPTURE(BM_read, data3, "test/data3.txt");


BENCHMARK_MAIN();