#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
  int n, i, j, info;
  double *A;

  if( argc<2 ) {
    fprintf(stderr,"usage: %s n\n",argv[0]);
    exit(-1);
  }
  sscanf(argv[1],"%d",&n);
  if( ( A = (double*) malloc(n*n*sizeof(double)) ) == NULL ) {
    fprintf(stderr,"Error en la reserva de memoria para la matriz A\n");
    exit(-1);
  }
  for( j=0; j<n; j++ ) {
    for( i=j; i<n; i++ ) {
      A[j*n+i] = ((double) rand()) / RAND_MAX;
    }
    A[j*n+j] += n;
  }
  dpotrf_( "L", &n, A, &n, &info );
  if( info != 0 ) {
    fprintf(stderr,"Error = %d en la descomposicion de Cholesky \n",info);
    exit(-1);
  }
  free(A);
}

