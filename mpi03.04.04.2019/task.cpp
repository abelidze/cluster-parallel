#include <iostream>
#include <mpi.h>

int main (int argc, char* argv[])
{
  int n, rank;
  char current = 127;
  char previous = 127;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    double startTime = MPI_Wtime();
    bool isDesc = true;
    std::cout << "Hello from main process " << rank << std::endl;
    for (int i = 1; i < n; ++i) {
      MPI_Recv(&current, 1, MPI_CHAR,
          MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      isDesc = isDesc && (current < previous);
      previous = current;
      std::cout << "Hello from process " << (int) current << std::endl;
    }
    std::cout << "Is descending? " << (isDesc ? "True" : "False") << std::endl;
    std::cout << "Execution finished in " << (MPI_Wtime() - startTime) << std::endl;
  } else {
    current = rank;
    MPI_Send(&current, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}