#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank, size, rbuf;
  int ranks[128], new_rank;
  MPI_Group group, new_group;
  MPI_Comm new_comm;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_group(MPI_COMM_WORLD, &group);

  for (int i = 0; i < size / 2; ++i) {
    ranks[i] = i;
  }

  if (rank < size / 2) {
    MPI_Group_incl(group, size / 2, ranks, &new_group);
  } else {
    MPI_Group_excl(group, size / 2, ranks, &new_group);
  }

  MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
  MPI_Allreduce(&rank, &rbuf, 1, MPI_INT, MPI_SUM, new_comm);
  MPI_Group_rank(new_group, &new_rank);

  std::cout << "rank = " << rank << ", newrank = " << new_rank << ", rbuf = " << rbuf << std::endl;

  MPI_Finalize();
  return 0;
}