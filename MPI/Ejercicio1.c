#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
    typedef struct {
    int i;
    double a;
    int b[10];
  } tipo_mensaje;

  tipo_mensaje m;
  int myproc;
  int nproc;
  int i;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myproc );
  MPI_Comm_size( MPI_COMM_WORLD, &nproc );
  MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_INT}, newtype;
  int lengths[3] = {1,1,10};
  MPI_Aint ad1,ad2,ad3,ad4,displs[3];
  MPI_Get_address(&m,&ad1);
  MPI_Get_address(&m.i,&ad2);
  MPI_Get_address(&m.a,&ad3);
  MPI_Get_address(&m.b[0],&ad4);
  displs[0] = ad2 - ad1;
  displs[1] = ad3 - ad1;
  displs[2] = ad4 - ad1;
  MPI_Type_create_struct(3,lengths,displs,types,&newtype);
  MPI_Type_commit(&newtype);

  MPI_Status status;

  if (myproc == 0) {
	  m.i = 1010;
	  m.a = 5;
	  int i;
	  for(i = 0; i < 10; i++)
	  	m.b[i] = i;
	MPI_Send(&m,1,newtype,1,123,MPI_COMM_WORLD);
  }
  else if (myproc == 1) {
	MPI_Recv(&m,1,newtype,0,123,MPI_COMM_WORLD,&status);
	printf("m");
	printf("i = %d\n",m.i );
	printf("a = %f\n", m.a);
	int i;
	int n = sizeof(m.b)/sizeof(int);
	printf("b = \n");
	for(i = 0; i < n; i++) {
		printf("[%d] = %d \n",i,m.b[i]);
	}
  }
  MPI_Finalize();
  return 0;
}

