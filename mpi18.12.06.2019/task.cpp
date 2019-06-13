#include <iostream>
#include <fstream>
#include <mpi.h>

#define BUFSIZE 100
#define FILE_NAME "file.txt"

void init_file()
{
  const int size = BUFSIZE * 5.5;
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
  int rank, num;
  char buf[BUFSIZE + 1];
  MPI_Status status;
  MPI_File fh;

  init_file();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_File_open(MPI_COMM_WORLD, FILE_NAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_set_view(fh, rank, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

  int sum = 0;
  do {
    MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
    MPI_Get_count(&status, MPI_CHAR, &num);
    buf[num] = 0;
    std::cout << "buf = " << buf << std::endl;
    sum += num;
  } while (num >= BUFSIZE);
  MPI_File_close(&fh);

  std::cout << "Sum = " << sum << std::endl;

  MPI_Finalize();
  std::remove(FILE_NAME);
  return 0;
}