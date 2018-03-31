#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define MAX_NUMEROS 1000
#define SOLUCION_ENCONTRADA 15
#define ESCLAVO_PREPARADO 10
#define BUSCAR_PRIMO 5
#define FIN 0


int primo(int n) {
  int p;
  int i, s;
  p = (n % 2 != 0 || n == 2);
  if (p) {
    s = sqrt(n);
    for (i = 3; p && i <= s; i += 2)
      if (n % i == 0) p = 0;
  }
  return p;
}

int main( int argc, char *argv[] ) {
  int myproc;
  int nproc;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myproc );
  MPI_Comm_size( MPI_COMM_WORLD, &nproc );

  

  MPI_Status status;

  if (myproc == 0) {
    srand(1111);
    int prim;
    int i = 0;
    int encontrado = 0;
    int esclavos = nproc - 1;
    while (esclavos > 0 ) {
        MPI_Recv(&prim,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if(status.MPI_TAG == SOLUCION_ENCONTRADA && encontrado == 0) {
            printf("Número primo: %d\n",prim);
            esclavos--;
            encontrado = 1;
        } else {
            if(!encontrado && i < MAX_NUMEROS) {
                int nRand = rand();
                MPI_Send(&nRand,1,MPI_INT,status.MPI_SOURCE,BUSCAR_PRIMO,MPI_COMM_WORLD);
                i++;
            } else {
                MPI_Send(&i,0,MPI_INT,status.MPI_SOURCE,FIN,MPI_COMM_WORLD);
                esclavos--;
            }            
        }
    }
  } else {
      int dataS;
      MPI_Send(&dataS,0,MPI_INT,0,ESCLAVO_PREPARADO,MPI_COMM_WORLD);
      int continuar = 1;
      while (continuar != 0) {
          int dataR;
          MPI_Recv(&dataR,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
          if(status.MPI_TAG == FIN)
              continuar = 0;
          else {
              int n = primo(dataR);
              if(n) {
                  MPI_Send(&dataR,1,MPI_INT,0,SOLUCION_ENCONTRADA,MPI_COMM_WORLD);
                  continuar = 0;
              } else {
                  MPI_Send(&dataS,0,MPI_INT,0,ESCLAVO_PREPARADO,MPI_COMM_WORLD);
                }
          }         
      }
  }
  MPI_Finalize();
  return 0;
}

