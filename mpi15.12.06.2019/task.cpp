#include <iostream>
#include <iomanip>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank, size;
  float a[10];
  char b[10], buf[100];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < 10; ++i) {
    a[i] = rank + 1.0;
    b[i] = rank == 0 ? 'a' : 'b';
  }

  std::cout << "rank = " << rank << "| BEFORE: a = ( " << std::fixed << std::setprecision(0);
  for (int i = 0; i < 10; ++i) {
    std::cout << a[i] << " ";
  }
  std::cout << "), b = " << b;

  int position = 0;
  if (rank == 0) {
    MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD);
    MPI_Pack(b, 10, MPI_CHAR, buf, 100, &position, MPI_COMM_WORLD);
    MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
  } else {
    MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
    MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(buf, 100, &position, b, 10, MPI_CHAR, MPI_COMM_WORLD);
  }

  std::cout << "| AFTER: a = ( ";
  for (int i = 0; i < 10; ++i) {
    std::cout << a[i] << " ";
  }
  std::cout << "), b = " << b << std::endl;

  MPI_Finalize();
  return 0;
}