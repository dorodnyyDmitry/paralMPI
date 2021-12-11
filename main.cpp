#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int main(int argc, char* argv[]){
  int world_rank, world_size, int_size, i;

  int N = argc - 1;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int startval = N * world_rank / world_size + 1;
  int endval = N * (world_rank+1) / world_size;
  int n = N / world_size;

  int sum = 0;
  
  #pragma omp parallel for num_threads(4)
  
  for (int i = startval; i <= endval; ++i){
    sum += atoi(argv[i]);
  }

  std::cout 
    << "Summ in process # " << world_rank << " : " << sum 
    << ", average: " << sum/n << "\n";

  int sum_all = 0;
  MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(world_rank == 0){
    std::cout 
      << "Total sum: " << sum_all 
      << ", avg: " << sum_all/N;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

}
