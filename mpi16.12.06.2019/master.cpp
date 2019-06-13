#include <iostream>
#include <mpi.h>

#define SLAVE_COUNT 5

int main(int argc, char* argv[])
{
  int size;
  int rank[SLAVE_COUNT];
  char cmd[10] = "./slave";
  MPI_Status status;
  MPI_Comm intercomm;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Comm_spawn(cmd, MPI_ARGV_NULL, SLAVE_COUNT, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);

  for (int i = 0; i < SLAVE_COUNT; ++i) {
    MPI_Recv(rank + i, 1, MPI_INT, i, i, intercomm, &status);
  }

  std::cout << "Working slaves: ";
  for (int i = 0; i < SLAVE_COUNT; ++i) {
    std::cout << rank[i] << " ";
  }
  std::cout << std::endl;

  if (SLAVE_COUNT > 2) {
    MPI_Recv(&size, 1, MPI_INT, 3, 3, intercomm, &status);
    std::cout << "Size from slave3 = " << size << std::endl;
  }

  MPI_Finalize();
  return 0;
}