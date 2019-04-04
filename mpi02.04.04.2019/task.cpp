#include <iostream>
#include <mpi.h>

int main (int argc, char* argv[])
{
  int n, rank;
  char message;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 6) {
    double startTime = MPI_Wtime();
    std::cout << "Hello from main process " << rank << std::endl;
    for (int i = 1; i < n; ++i) {
      MPI_Recv(&message, 1, MPI_CHAR,
          MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      std::cout << "Hello from process " << (double) message / rank << std::endl;
    }
    std::cout << "Execution finished in " << (MPI_Wtime() - startTime) << std::endl;
  } else {
    message = rank;
    MPI_Send(&message, 1, MPI_CHAR, 6, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}