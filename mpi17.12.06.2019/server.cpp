#include <iostream>
#include <mpi.h>

#define SLAVE_COUNT 5

int main(int argc, char* argv[])
{
  int size, payload;
  char port[MPI_MAX_PORT_NAME];
  MPI_Status status;
  MPI_Comm intercomm;

  MPI_Init(&argc, &argv);
  MPI_Open_port(MPI_INFO_NULL, port);
  std::cout << "Port: " << port << std::endl;

  MPI_Comm_accept(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
  MPI_Recv(&payload, 1, MPI_INT, 0, 0, intercomm, &status);
  MPI_Comm_free(&intercomm);
  MPI_Close_port(port);

  std::cout << "Client sent: " << payload << std::endl;

  MPI_Finalize();
  return 0;
}