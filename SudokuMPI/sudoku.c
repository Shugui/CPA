#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include "sudoku.h"

#define A(a,b) A[(a)*81+(b)]
#define B(a,b) B[(a)*81+(b)]
#define data(a,b) data[(a)*81+(b)]


#define FIN 1
#define EXITO 2
#define FRACASO 3
#define ESCLAVO_PREPARADO 4
#define BUSCAR_SOL 5
#define EXITO_OTRO 6


int main( int argc, char *argv[] ) {

  int numero_nodos;
  int encontrado;
  int data[81];
  int A[3000*81];
  int B[3000*81];

  int myproc;
  int nprocs;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myproc );
  MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
  MPI_Status status;

  MPI_Datatype tablero;
  MPI_Type_contiguous( 81, MPI_INT, &tablero );
  MPI_Type_commit( &tablero );

  if(myproc == 0) {
    int mascara[81];
    int sol[81];
    int i, j;
    int numNiveles = 8;

 

    printf("tablero inicial: \n");
    init_sudoku("normal",sol);
    prin_sudoku(sol);
    for( i = 1; i <= 9; i++ ) {
      for( j = 1; j <= 9; j++ ) {
       mascara(i,j) = sol(i,j) != 0;
      }
    }
    //Intro
    for(i = 0; i<81; i++)
      A[i]= sol[i];
    numero_nodos = 1;

    int nivel,k,indA,indB;
    int fila, columna, valor;


    for(nivel = 1; nivel <= numNiveles; nivel++){
      indB = 0;

      for(indA = 0; indA < numero_nodos; indA++) {
        k = 0;
        while(k < 81 && A(indA,k) != 0){
          k++;
        }
        if(k < 81) {
          fila = 1 + k/9;
          columna = 1 + k%9;
          for(valor  = 1; valor <= 9; valor ++) {
            A(indA,k) = valor;
            if(es_factible(fila, columna, &A(indA,0))) {
              //Copiar tablero con nuevo factible.
              for(i=0; i < 81; i++) {
                B(indB,i) = A(indA,i);

              }
              indB = indB + 1;
            } 
          }
        }

      }
      for(i = 0; i < indB; i++) 
        for(j = 0; j < 81; j++) 
          A(i,j) = B(i,j);
      numero_nodos = indB; 
    }

    //Codigo maestro
    i = 0;
    encontrado = 0;
    int esclavos = nprocs - 1;
    while (esclavos > 0) {
      MPI_Recv(&data,1,tablero,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      if(status.MPI_TAG == EXITO) {
        prin_sudoku(data);
        encontrado = 1;
        esclavos--;

      } else {
        if(!encontrado && i < numero_nodos) {
          MPI_Send(&A(i,0) ,1,tablero,status.MPI_SOURCE,BUSCAR_SOL,MPI_COMM_WORLD);
          i++;
        }
        else {
          MPI_Send(&data,0,tablero,status.MPI_SOURCE,FIN,MPI_COMM_WORLD);
          esclavos--;
        }
      }
      if(status.MPI_TAG == EXITO_OTRO){
        MPI_Send(&data,0,tablero,status.MPI_SOURCE,FIN,MPI_COMM_WORLD);
          esclavos--;
      } 
    }
  }else{
    MPI_Send(&data,0,tablero,0,ESCLAVO_PREPARADO,MPI_COMM_WORLD);
    int continuar = 1;
    while (continuar != 0) {
      MPI_Recv(&data,1,tablero,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      if(status.MPI_TAG == FIN)
        continuar = 0;
      else {
          encontrado = 0;
          int mascara[81];
          int i;
          for(i = 0; i < 81; i++) 
            mascara[i] = data(0,i) != 0;
          sudoku_sol(1,1,data,mascara);
        
        if(encontrado) {
          MPI_Send(&data,1,tablero,0,EXITO_OTRO,MPI_COMM_WORLD);
          continuar = 0;
        } else {
          MPI_Send(&data,0,tablero,0,FRACASO,MPI_COMM_WORLD);

        }

      }

    }
  }

  MPI_Finalize();
  return 0;
}

void sudoku_sol( int i, int j, int sol[81], int mascara[81] ) {
   int k;
   if( mascara(i, j) == 0 ) {
      for( k = 1; k <= 9; k++ ) {
         sol( i, j ) = k;                                
         if( es_factible( i, j, sol ) ) {
            if( i == 9 && j == 9 ) {
               printf("Solucion: \n");
               prin_sudoku(sol);
            }
            if( i < 9 && j == 9 ) {
               sudoku_sol ( i+1, 1, sol, mascara );
            }
            if( i <= 9 && j < 9 ) {
               sudoku_sol( i, j+1, sol, mascara );
            }
         }
      } 
      sol(i, j) = 0;                             
   } else { 
      if( i == 9 && j == 9 ) {
         printf("Solucion: \n");
         prin_sudoku(sol);
      }
      if( i < 9 && j == 9 ) {
         sudoku_sol ( i+1, 1, sol, mascara );
      }
      if( i <= 9 && j < 9 ) {
         sudoku_sol( i , j+1, sol, mascara );
      }
   }
}

