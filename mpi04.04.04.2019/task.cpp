#include <iostream>
#include <mpi.h>

const int NTIMES = 100;

int main (int argc, char* argv[])
{
  int rank, length;
  double startTime, endTime;
  char* name = new char;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &length);

  startTime = MPI_Wtime();
  for (int i = 1; i < NTIMES; ++i) {
    endTime = MPI_Wtime();
  }

  std::cout << "processor " << name
            << ", process " << rank
            << ", time = " << (endTime - startTime) / NTIMES << std::endl;

  MPI_Finalize();
  return 0;
}