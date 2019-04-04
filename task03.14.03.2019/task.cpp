#include <cstdlib>
#include <iostream>
#include <vector>
#include <omp.h>

#include "include/table_printer.h"

struct result_t
{
  result_t()
    : result(0),
      threads(1),
      time(.0) { }

  result_t(int r, int th, double dt)
    : result(r),
      threads(th),
      time(dt) { }

  int result;
  int threads;
  double time;
};

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::printf("Use: %s <vector_size>\n", argv[0]);
    return 0;
  }

  const int MAX_THREADS = omp_get_max_threads();
  std::vector<std::pair<int, std::vector<result_t>>> results;
  double startTime, endTime;

  for (int k = 1; k < argc; ++k) {
    int size = std::atoi(argv[k]);
    results.emplace_back(size, std::vector<result_t>(0));
    std::vector<int> v1(size, 1);
    std::vector<int> v2(size, 1);
    for (int count = 1; count <= MAX_THREADS; ++count) {
      startTime = omp_get_wtime();
      int result = 0;
      #pragma omp parallel for reduction (+: result) num_threads(count)
      for (int i = 0; i < size; ++i) {
        result += v1[i] * v2[i];
        // std::printf("Thread %d processed component #%d\n", omp_get_thread_num(), i);
      }
      endTime = omp_get_wtime();
      results.back().second.emplace_back(result, count, (endTime - startTime) * 1000);
    }
  }

  bprinter::TablePrinter tp(&std::cout);
  tp.AddColumn("Threads", 9);
  tp.AddColumn("Result", 10);
  tp.AddColumn("Time (ms)", 18);

  for (auto& group : results) {
    std::cout << "Dimension = " << group.first << std::endl;

    tp.PrintHeader();
    for (auto& entry : group.second) {
      tp << entry.threads << entry.result << entry.time;
    }
    tp.PrintFooter();
  }

  return 0;
} 