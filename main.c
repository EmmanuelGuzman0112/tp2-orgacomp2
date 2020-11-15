#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include <unistd.h>

#include <time.h>

int main()
{
    char pathImagen[100];
    clock_t comienzo, fin;

	//Creamos csv
	FILE *out = fopen("resultadosEnTiempos.csv", "w+");
	fprintf(out, "%s  %c %s %c  %s %c  %s", "Path imagen", ',', "Comienzo", ',', "Fin", ',', "Duracion\n");
	fclose(out);
	
	printf("\nIngrese el path e imagen\n");
	scanf("%s", pathImagen); //Capturo la imagen

	//Ejemplo:
    //home/lapassesungs/Escritorio/TP2/imagenes/imagen1.jpg
	if(access(pathImagen, F_OK) != -1) //Chequeo la existencia del file
	{
		printf("\e[1;1H\e[2J"); //Limpio la consola
		printf("La imagen ingresada %s es correcta\n", pathImagen);

        comienzo = clock();
        //Llamada a la funcion ACA
        fin = clock();

        FILE *out = fopen("resultadosEnTiempos.csv", "a"); //Abro el csv

        int tiempo = fin-comienzo;
        
        fprintf(out, "%s %c %Lf %c %Lf %c %Lf  %s", 
        pathImagen, ',', (long double)comienzo/CLOCKS_PER_SEC, ',', (long double)fin/CLOCKS_PER_SEC, ',', (long double)tiempo/CLOCKS_PER_SEC, "\n");

        fclose(out);
        
        //Imprime tiempo de cada filtro
        printf("\nTiempo consumido: %ld \n", fin-comienzo);
	}
	else
	{
		printf("\nLa imagen ingresada no es correcta\n");
    }

	return 0;
}