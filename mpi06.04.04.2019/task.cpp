#include <cstdlib>
#include <iostream>
#include <vector>
#include <mpi.h>

#include "include/table_printer.h"

int main (int argc, char* argv[])
{
  if (argc < 2) {
    std::printf("Use: %s <vector_size>\n", argv[0]);
    return 0;
  }

  const int VECTOR_SIZE = std::atoi(argv[1]);

  int rank;
  int procCount;
  double startTime, endTime;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int len = VECTOR_SIZE / procCount;
  if (rank == 0) {
    len += VECTOR_SIZE % procCount;
  }

  std::vector<int> a(len, rank + 1);
  std::vector<int> b(len, rank + 1);

  int result = 0;
  startTime = MPI_Wtime();
  for (int i = 0; i < len; ++i) {
    result += a[i] * b[i];
  }

  if (rank == 0) {
    int other = 0;
    int total = result;
    std::cout << "Processors count = " << procCount << std::endl;
    for (int i = 1; i < procCount; ++i) {
      MPI_Recv(&other, 1, MPI_INT,
          MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
      total += other;
      // std::cout << "Recv " << other << std::endl;
    }
    endTime = MPI_Wtime();

    bprinter::TablePrinter tp(&std::cout);
    tp.AddColumn("(a, b) = ?", 12);
    tp.AddColumn("Size", 12);
    tp.AddColumn("Time (ms)", 18);

    tp.PrintHeader();
    tp << total << VECTOR_SIZE << (endTime - startTime) * 1000;
    tp.PrintFooter();
  } else {
    MPI_Send(&result, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}