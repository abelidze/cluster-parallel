#include <iostream>
#include <algorithm>
#include <string>
#include <mpi.h>
#define N 1000

void max(void* a, void* b, int* l, MPI_Datatype* type)
{
  int* x = (int*) a;
  int* y = (int*) b;
  for (int i = 0; i < *l; ++i) {
    y[i] = std::max(x[i], y[i]);
  }
}

int main(int argc, char* argv[])
{
  int rank, size;
  int a[N];
  int b[N];
  int c[N];
  MPI_Op op;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < N; ++i) {
    a[i] = i + rank +1;
    b[i] = 0;
    c[i] = 0;
  }

  std::cout << "process " << rank << ", a[0] = " << a[0] << ", a[N - 1] = " << a[N - 1] << std::endl;

  MPI_Op_create(&max, 1, &op);
  MPI_Reduce(a, b, N, MPI_INT, op, 0, MPI_COMM_WORLD);
  MPI_Reduce(a, c, N, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Op_free(&op);

  if (rank == 0) {
    std::string text = "success";
    for (int i = 0; i < N; ++i) {
      if (b[i] != c[i]) {
        text = "error";
        break;
      }
    }
    std::cout << text << ", b[0] = " << b[0] << ", b[N - 1] = " << b[N - 1] << std::endl;
  }

  MPI_Finalize();
  return 0;
}