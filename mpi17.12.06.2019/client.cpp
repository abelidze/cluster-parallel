#include <iostream>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank;
  char port[MPI_MAX_PORT_NAME];
  MPI_Comm intercomm;

  std::strcpy(port, argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Send(&rank, 1, MPI_INT, 0, 0, intercomm);

  MPI_Finalize();
  return 0;
}