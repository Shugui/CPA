#include <stdio.h>
#include <stdlib.h>



int ordenaArray (const void * _a, const void * _b)
{
    int *a, *b;
        
        a = (int *) _a;
        b = (int *) _b;
        
        return (*a - *b);
}




void main ()
{
	char nombre[15] = "fichero1.txt";
	int elements, c, i = 0;
	int v[100];
	FILE *file = fopen( nombre , "r");
	if (file)
	{
	    while ((fscanf(file, "%d", &v[i])!=EOF) && (i <100))
        i++;
	}else
	{
		printf("El fichero  no existe.\n");	
	}
	printf("El contenido del fichero ordenado es: \n");
	qsort(v,100, sizeof(int), &ordenaArray );	
	
	for( i = 0 ; i < 100; i++ ) 
   {   
      printf("%d ", v[i]);
   }

}