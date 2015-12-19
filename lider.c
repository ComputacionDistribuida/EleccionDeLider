#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 128
#define TAG 0

int main(int argc, char *argv[])
{
  int numero;
  int lider;
  char buff[BUFSIZE];
  char auxbuff[BUFSIZE];
  char *ptr;
  int size;
  int rank;
  int i;
  MPI_Status stat;

  MPI_Init(&argc,&argv); 
  MPI_Comm_size(MPI_COMM_WORLD,&size); 
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  srand(MPI_Wtime()*1000000);	
  numero = 1+rand()%100;
  lider = rank;
  printf("%d: %d\n",rank,numero);

  if(rank == 0)
  {
      sprintf(buff, "%d.%d\n",numero,rank);
      MPI_Send(buff, BUFSIZE, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD);
      MPI_Recv(buff, BUFSIZE, MPI_CHAR, size-1, TAG, MPI_COMM_WORLD, &stat);
      printf("%d: %s\n",rank,buff);
      MPI_Send(buff, BUFSIZE, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD);     
  } else if (rank == size-1)
  {
    MPI_Recv(buff, BUFSIZE, MPI_CHAR, rank-1, TAG, MPI_COMM_WORLD, &stat);
    strcpy(auxbuff,buff);
    ptr = strtok(auxbuff,".");     
    if (numero <= atoi(ptr))
    {
      numero = atoi(ptr);
      ptr = strtok(NULL, ".");
      lider = atoi(ptr);
    }
    sprintf(buff,"El lider es %d con %d",lider,numero);
    printf("%d: %s\n",rank,buff);
    MPI_Send(buff, BUFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);    
  }
  else 
  {
    MPI_Recv(buff, BUFSIZE, MPI_CHAR, rank-1, TAG, MPI_COMM_WORLD, &stat);
    strcpy(auxbuff,buff);
    ptr = strtok(auxbuff,".");     
    if (numero <= atoi(ptr))
    {
      MPI_Send(buff, BUFSIZE, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD);
    }
    else
    {
      sprintf(buff, "%d.%d\n",numero,rank);
      MPI_Send(buff, BUFSIZE, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD);
    }
    MPI_Recv(buff, BUFSIZE, MPI_CHAR, rank-1, TAG, MPI_COMM_WORLD, &stat);
    printf("%d: %s\n",rank,buff);
    if (rank != size-2)
    {
      MPI_Send(buff, BUFSIZE, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD); 
    }
  }

  MPI_Finalize(); 
  return 0;
}

