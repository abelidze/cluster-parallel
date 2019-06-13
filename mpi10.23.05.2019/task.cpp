#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
  int rank, size, prev, next;
  float rbuf[2], sbuf[2];
  MPI_Request reqs[4];
  MPI_Status stats[4];

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  prev = (rank == 0) ? size - 1 : rank - 1;
  next = (rank == size - 1) ? 0 : rank + 1;

  MPI_Recv_init(&rbuf[0], 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
  MPI_Recv_init(&rbuf[1], 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &reqs[1]);
  MPI_Send_init(&sbuf[0], 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
  MPI_Send_init(&sbuf[1], 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &reqs[3]);

  for (int i = 1; i <= 4; ++i) {
    sbuf[0] = rank * i;
    sbuf[1] = rank * i;

    MPI_Startall(4, reqs);
    std::cout << "i = " << i << ", " << rank << " send: prev=" << sbuf[0] << ", next=" << sbuf[1] << std::endl;

    MPI_Waitall(4, reqs, stats);
    std::cout
        << "i = " << i << ", " << rank
        << " recv: prev=" << rbuf[0]
        << ", next=" << rbuf[1]
        << (rbuf[0] == prev * i && rbuf[1] == next * i ? ", valid" : ", error")
        << std::endl;
  }

  for (int i = 0; i < 4; ++i) {
    MPI_Request_free(&reqs[i]);
  }

  MPI_Finalize();
  return 0;
}
