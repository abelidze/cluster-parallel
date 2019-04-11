#include <iostream>
#include <mpi.h>

int main (int argc, char* argv[])
{
  int rank;
  int size;
  int ibuf;
  float fbuf;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ibuf = rank;
  fbuf = 1.0 * rank;

  switch (rank) {
    case 1: MPI_Send(&ibuf, 1, MPI_INT, 0, 5, MPI_COMM_WORLD); break;
    case 2: MPI_Send(&fbuf, 1, MPI_FLOAT, 0, 6, MPI_COMM_WORLD); break;
    case 0:
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (status.MPI_TAG == 5) {
        MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
        MPI_Recv(&fbuf, 1, MPI_FLOAT, 2, 6, MPI_COMM_WORLD, &status);
        std::cout << "Process 0 recv "
                  << ibuf << " from tag 5, "
                  << fbuf << " from tag 6"
                  << std::endl;
      } else if (status.MPI_TAG == 6) {
        MPI_Recv(&fbuf, 1, MPI_FLOAT, 2, 6, MPI_COMM_WORLD, &status);
        MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
        std::cout << "Process 0 recv "
                  << fbuf << " from tag 6 "
                  << ibuf << " from tag 5"
                  << std::endl;
      }
      break;
  }

  MPI_Finalize();
  return 0;
}