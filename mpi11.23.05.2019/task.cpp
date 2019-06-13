#include <iostream>
#include <algorithm>
#include <string>
#include <mpi.h>
#define N 500

int main(int argc, char* argv[])
{
  int rank, size;
  int a[N][N];
  int b[N][N];
  int c[N][N];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      a[i][j] = i + j;
      b[i][j] = i + j;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  double startTime = MPI_Wtime();
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  double endTime = MPI_Wtime();
  std::cout << "process " << rank << " time (ms) = " << (endTime - startTime) * 1000 << std::endl;

  MPI_Finalize();
  return 0;
}