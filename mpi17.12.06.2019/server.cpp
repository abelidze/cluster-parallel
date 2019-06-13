#include <iostream>
#include <string>
#include <mpi.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
  char port[MPI_MAX_PORT_NAME];
  char buffer[BUFFER_SIZE];

  MPI_Init(&argc, &argv);
  MPI_Open_port(MPI_INFO_NULL, port);
  {
    std::cout << "PortName: " << port << std::endl << "Waiting for incomming connection..." << std::endl;

    MPI_Comm intercomm;
    MPI_Comm_accept(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    std::cout << "Client connected." << std::endl;

    std::cout << "Enter message to send: ";
    std::string message;
    std::cin >> message;
    MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, 0, 0, intercomm);

    MPI_Status status;
    MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, intercomm, &status);
    std::cout << "Received from client: " << buffer << std::endl;

    MPI_Comm_free(&intercomm);
  }
  MPI_Close_port(port);
  MPI_Finalize();
  return 0;
}