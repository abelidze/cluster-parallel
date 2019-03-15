#include <cstdio>
#include <omp.h>

#define MAX_N 10


int main(int argc, char* argv[])
{
  int sum;

  int A[MAX_N];
  for (int i = 0; i < MAX_N; ++i) {
    A[i] = 1;
  }

  // TASK#1
  std::printf("TASK #1\n");

  sum = 0;
  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < MAX_N; ++i) {
      sum += A[i];
      std::printf("Thread %d processed A[%d]\n", omp_get_thread_num(), i);
    }
  }
  std::printf("SUM(A, 0, %d) = %d\n\n", MAX_N, sum);

  sum = 0;
  #pragma omp parallel reduction (+: sum)
  {
    #pragma omp for
    for (int i = 0; i < MAX_N; ++i) {
      sum += A[i];
      std::printf("Thread %d processed A[%d]\n", omp_get_thread_num(), i);
    }
  }
  std::printf("SUM(A, 0, %d) = %d\n\n", MAX_N, sum);
  // End of TASK#1

  // TASK#2
  std::printf("TASK #2\n");

  sum = 0;
  #pragma omp parallel
  {
    int local = 0;
    #pragma omp for
    for (int i = 0; i < MAX_N; ++i) {
      local += A[i];
      std::printf("Thread %d processed A[%d]\n", omp_get_thread_num(), i);
    }
    #pragma omp atomic
    sum += local;
  }
  std::printf("SUM(A, 0, %d) = %d\n\n", MAX_N, sum);
  // End of TASK#2

  // TASK#3
  std::printf("TASK #3\n");

  sum = 0;
  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < MAX_N; ++i) {
      #pragma omp critical
      sum += A[i];
      std::printf("Thread %d processed A[%d]\n", omp_get_thread_num(), i);
    }
  }
  std::printf("SUM(A, 0, %d) = %d\n", MAX_N, sum);
  // End of TASK#3


  std::printf("Execution finished");

  return 0;
} 