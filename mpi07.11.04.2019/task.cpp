#define WITHOUT_NUMPY

#include <Python.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <mpi.h>
#include <cmath>

#include "include/matplotlibcpp.h"

// 100 Mb = 104857600 bytes = 100 x 131072 x 8
#define STEPS 100
#define OFFSET 131072
#define ATTEMPTS 10

namespace plt = matplotlibcpp;

char* fs_readable(double size, char* buf) {
  int i = 0;
  const char* units[] = { "B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
  while (size > 1024) {
    size /= 1024;
    ++i;
  }
  sprintf(buf, "%.*f %s", i, size, units[i]);
  return buf;
}

int main (int argc, char* argv[])
{
  int rank;
  double startTime;
  double average;
  MPI_Status status;

  std::vector<double> x(STEPS + 1, 0), t(STEPS + 1, 0), bw(STEPS + 1, 0);
  const int elementSize = sizeof(long long);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i <= STEPS; ++i) {
    const size_t size = i * OFFSET;
    auto payload = new long long[size];
    x[i] = size * elementSize;

    average = 0;
    for (int k = 0; k < ATTEMPTS; ++k) {
      switch (rank) {
        case 0:
        {
          startTime = MPI_Wtime();
          MPI_Send(payload, size, MPI_LONG_LONG_INT, 1, 5, MPI_COMM_WORLD);
          MPI_Recv(payload, size, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
          average += (MPI_Wtime() - startTime) * 1000;
          break;
        }

        case 1:
        {
          MPI_Recv(payload, size, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
          MPI_Send(payload, size, MPI_LONG_LONG_INT, 0, 5, MPI_COMM_WORLD);
          break;
        }
      }
    }
    delete[] payload;

    if (rank == 0) {
      t[i] = average / ATTEMPTS / 2;
      bw[i] = 2 * x[i] * ATTEMPTS / average;

      if (size == 0) {
        std::cout << "Latency = " << t[i] << "ms" << std::endl;
      } else {
        std::cout << t[i]  << "ms bw=" << bw[i] << std::endl;
      }
    }
  }

  if (rank == 0) {
    std::vector<double> ticks(STEPS);
    std::vector<std::string> labels(STEPS);
    const double maxSize = STEPS * OFFSET * elementSize;
    const int steps = std::min(STEPS, 12);

    for (int i = 0; i < steps; ++i) {
      ticks[i] = maxSize * (i + 1) / steps;
      labels[i] = fs_readable(ticks[i], new char[1024]);
    }

    plt::figure_size(1200, 780);

    plt::xlim(0, STEPS * OFFSET * elementSize);
    plt::title("Network analysis - time");
    plt::named_plot("t(size)", x, t);
    plt::xlabel("size, bytes");
    plt::ylabel("time, ms");
    plt::xticks(ticks, labels);
    plt::legend();
    plt::save("time.png");

    plt::clf();
    plt::xlim(0, STEPS * OFFSET * elementSize);
    plt::title("Network analysis - bandwidth");
    plt::named_plot("bandwidth(size)", x, bw);
    plt::xlabel("size, bytes");
    plt::ylabel("bandwidth, byte/s");
    plt::xticks(ticks, labels);
    plt::legend();
    plt::save("bandwidth.png");
  }

  MPI_Finalize();
  return 0;
}