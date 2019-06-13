#include <iostream>
#include <fstream>
#include <mpi.h>

#define BUFSIZE 100
#define FILE_NAME "file.txt"

void init_file()
{
  const int size = BUFSIZE * 256;
  char ch = 'a';
  std::ofstream file(FILE_NAME);
  for (int i = 0; i < size; ++i) {
    file << ch;
    ch = ch == 'z' ? 'a' : ch + 1;
  }
  file.close();
}

int main(int argc, char* argv[])
{
  int rank;
  char buf[BUFSIZE + 1];
  MPI_Status status;
  MPI_File fh;

  init_file();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_File_open(MPI_COMM_WORLD, FILE_NAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_set_view(fh, rank * BUFSIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
  MPI_File_read_all(fh, buf, BUFSIZE, MPI_CHAR, MPI_STATUS_IGNORE);
  buf[BUFSIZE] = 0;
  std::cout << "process " << rank << ", buf = " << buf << std::endl;

  MPI_Finalize();
  std::remove(FILE_NAME);
  return 0;
}