#include <stdio.h>
#include "sudoku.h"


int es_factible( int i, int j, int sol[81] ) {
   int valido;
   int k, l;

   //printf("es_factible (%d,%d)\n",i,j);
   valido = 1;
   k = 1;
   while( k <= 9 && valido ) {                   //Comprobamos la columna
      if( sol( i, j ) == sol( k, j ) && k != i ) {
         valido = 0;
      }
      k = k + 1;
   }
   //printf("valido = %d\n",valido);
   l = 1;
   while ( l <= 9 && valido ) {                   //Comprobamos la fila
      if( sol( i, j ) == sol( i, l ) && l != j ) {
         valido = 0;
      }
      l = l + 1;
   }
   //printf("valido = %d\n",valido);
   k = correspondencia3x3( i );
   l = correspondencia3x3( j );  //Comprobamos el subgrupo de 3x3
   //printf("k = %d\n",k);
   //printf("l = %d\n",l);
   while( ( k < correspondencia3x3( i ) + 3 ) && valido ) { 
      while( ( l < correspondencia3x3( j ) + 3 ) && valido ) { 
         if( ( sol( i, j ) == sol( k, l ) ) && i != k && j != l ) { 
            valido = 0;
         }
         l = l + 1;
      }
      k = k + 1;
      l =  correspondencia3x3( j );
   }
   //printf("valido = %d\n",valido);
   /* Bucle de consumo */
   int outer = 300;
   int inner = 300;
   int o, p;
   int x;
   for( o=0; o<outer; o++ ) {
     for( p=0; p<inner; p++ ) {
       x = ( (x && !(x+(o&p))) || (!x && (x+(o&p))) );
     }
   }
   return valido;
}

int correspondencia3x3( int i ) {
   int k;
   int resultado;

   if( i % 3 == 0 ) { 
      k = ( i / 3 );
   } else {
      k = ( i / 3 ) + 1;
   }
   switch( k ) {
      case 1 : resultado = 1; break;
      case 2 : resultado = 4; break;
      case 3 : resultado = 7; break;
   }
   return resultado;
}

void prin_sudoku( int sol[81] ) {
  int i, j;
  for( i = 1; i <= 9; i++ ) {
    for( j = 1; j <= 9; j++ ) {
       printf(" %d", sol(i,j));
    }
    printf(" \n");
  }

}

