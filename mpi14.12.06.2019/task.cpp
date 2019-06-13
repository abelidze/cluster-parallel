#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank, size, new_rank;
  MPI_Comm new_comm;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm_split(MPI_COMM_WORLD, rank % 6, size - rank, &new_comm);
  MPI_Comm_rank(new_comm, &new_rank);

  std::cout << "rank = " << rank << ", newrank = " << new_rank << std::endl;

  MPI_Comm_free(&new_comm);

  MPI_Finalize();
  return 0;
}