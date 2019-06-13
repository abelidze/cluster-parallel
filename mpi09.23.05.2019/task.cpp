#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank, size, prev, next;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int buf[2];
  MPI_Request reqs[4];
  MPI_Status stats[4];

  prev = (rank == 0) ? size - 1 : rank - 1;
  next = (rank == size - 1) ? 0 : rank + 1;

  MPI_Irecv(&buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
  MPI_Irecv(&buf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
  MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
  MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
  MPI_Waitall(4, reqs, stats);

  std::cout << rank << ": prev=" << buf[0] << ", next=" << buf[1] << std::endl;

  MPI_Finalize();
  return 0;
}
