#include <cstdlib>
#include <iostream>
#include <vector>
#include <mpi.h>

#include "include/table_printer.h"

int sum_reduce(const std::vector<int>& v, int proc_id, int proc_count)
{
  int len = v.size() / proc_count;
  if (proc_id == proc_count - 1) {
    len += v.size() % proc_count;
  }

  int sum = 0;
  int offset = proc_id * (v.size() / proc_count);
  for (int i = 0; i < len; ++i) {
    sum += v[offset + i];
  }

  int result = 0;
  MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return result;
}

int sum_double(const std::vector<int>& v, int proc_id, int proc_count)
{
  int left = 2 * proc_id + 1;
  int right = 2 * proc_id + 2;

  if (proc_id == 0) {
    int left_size = v.size() / 2;
    int right_size = v.size() - left_size;
    int left_sum = 0;
    int right_sum = 0;

    MPI_Send(v.data(), left_size, MPI_INT, left, 0, MPI_COMM_WORLD);
    MPI_Send(v.data() + left_size, right_size, MPI_INT, right, 0, MPI_COMM_WORLD);
    MPI_Recv(&left_sum, 1, MPI_INT, left,  MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&right_sum, 1, MPI_INT, right, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return left_sum + right_sum;
  } else {

    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    int count = 0;
    MPI_Get_count(&status, MPI_INT, &count);

    int* buf = new int[count];
    MPI_Recv(buf, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    int left_size = count / 2;
    int right_size = count - left_size;

    int left_sum = 0, right_sum = 0;

    if (left < proc_count) {
      MPI_Send(buf, left_size, MPI_INT, left, 0, MPI_COMM_WORLD);
      MPI_Recv(&left_sum, 1, MPI_INT, left,  MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else for (int i = 0; i < left_size; i++) {
      left_sum += buf[i];
    }

    if (right < proc_count) {
      MPI_Send(buf + left_size, right_size, MPI_INT, right, 0, MPI_COMM_WORLD);
      MPI_Recv(&right_sum, 1, MPI_INT, right, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else for (int i = left_size; i < count; i++) {
      right_sum += buf[i];
    }

    int sum = left_sum + right_sum;
    MPI_Send(&sum, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    delete[] buf;
  }
  return 0;
}

int main (int argc, char* argv[])
{
  if (argc < 2) {
    std::printf("Use: %s <vector_size>\n", argv[0]);
    return 0;
  }
  const int VECTOR_SIZE = std::atoi(argv[1]);

  int rank, size;
  std::vector<int> a(VECTOR_SIZE, 1);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double reduce_time = MPI_Wtime();
  int reduce_result = sum_reduce(a, rank, size);
  reduce_time = MPI_Wtime() - reduce_time;

  double double_time = MPI_Wtime();
  int double_result = sum_double(a, rank, size);
  double_time = MPI_Wtime() - double_time;

  if (rank == 0) {
    std::cout << "Processors count = " << size << std::endl;
    bprinter::TablePrinter tp(&std::cout);
    tp.AddColumn("size(a)", 12);
    tp.AddColumn("sum_reduce(a) = ?", 18);
    tp.AddColumn("sum_double(a) = ?", 18);
    tp.AddColumn("ReduceTime (ms)", 18);
    tp.AddColumn("DoubleTime (ms)", 18);

    tp.PrintHeader();
    tp << VECTOR_SIZE
       << reduce_result
       << double_result
       << reduce_time * 1000
       << double_time * 1000;
    tp.PrintFooter();
  }

  MPI_Finalize();
  return 0;
}