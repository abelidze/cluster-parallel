#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

#include "include/table_printer.h"

struct result_t
{
  result_t()
    : pattern("none"),
      threads(1),
      boost(.0),
      time(.0) { }

  result_t(const char* r, int th,double b, double dt)
    : pattern(r),
      threads(th),
      boost(b),
      time(dt) { }

  const char* pattern;
  int threads;
  double boost;
  double time;
};

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::printf("Use: %s <matrix_size>\n", argv[0]);
    return 0;
  }

  const int MAX_THREADS = omp_get_max_threads();
  std::vector<std::pair<int, std::vector<result_t>>> results;
  double startTime, endTime;

  for (int n = 1; n < argc; ++n) {
    int size = std::atoi(argv[n]);
    results.emplace_back(size, std::vector<result_t>(0));

    std::vector< std::vector<int> > A(size);
    std::vector< std::vector<int> > B(size);
    std::vector< std::vector<int> > C(size);
    for (int i = 0; i < size; ++i) {
      A[i].resize(size);
      B[i].resize(size);
      C[i].resize(size);
      for (int j = 0; j < size; ++j) {
        A[i][j] = (i + j) % 2;
        B[i][j] = (i + j + 1) % 2;
        C[i][j] = 0;
      }
    }

    // PATTERN I-J-K
    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          for (int k = 0; k < size; ++k) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "i -> j -> k",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN I-J-K

    // PATTERN I-K-J
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        C[i][j] = 0;
      }
    }

    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
          for (int j = 0; j < size; ++j) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "i -> k -> j",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN I-K-J

    // PATTERN K-I-J
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        C[i][j] = 0;
      }
    }

    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
          for (int j = 0; j < size; ++j) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "k -> i -> j",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN K-I-J

    // PATTERN K-J-I
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        C[i][j] = 0;
      }
    }

    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int k = 0; k < size; ++k) {
        for (int j = 0; j < size; ++j) {
          for (int i = 0; i < size; ++i) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "k -> j -> i",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN K-J-I

    // PATTERN J-I-K
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        C[i][j] = 0;
      }
    }

    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
          for (int k = 0; k < size; ++k) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "j -> i -> k",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN J-I-K

    // PATTERN J-K-I
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        C[i][j] = 0;
      }
    }

    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for shared(A, B, C) num_threads(count)
      for (int j = 0; j < size; ++j) {
        for (int k = 0; k < size; ++k) {
          for (int i = 0; i < size; ++i) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
      endTime = omp_get_wtime();
      double time = (endTime - startTime) * 1000;
      results.back().second.emplace_back(
          "j -> k -> i",
          count,
          count > 1 ? results.back().second[0].time / time : 1,
          time
        );
    }
    // End of PATTERN J-K-I
  }

  bprinter::TablePrinter tp(&std::cout);
  tp.AddColumn("Patterns", 12);
  tp.AddColumn("Threads", 8);
  tp.AddColumn("Time (ms)", 16);
  tp.AddColumn("T1 / T", 12);

  for (auto& group : results) {
    std::cout << "VectorSize = " << group.first << std::endl;

    tp.PrintHeader();
    for (auto& entry : group.second) {
      if (entry.threads == 1) {
        tp << "pattern" << bprinter::endl();
      }
      tp << entry.pattern << entry.threads << entry.time << entry.boost;
    }
    tp.PrintFooter();

    std::cout << std::endl;
  }

  return 0;
} 