#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

void enmascarar_c(unsigned char *, unsigned char *, unsigned char *, int);

int main()
{
    char pathImagen1[100], pathImagen2[100], pathMask[100];
    clock_t comienzo, fin;

	//Creamos csv
	FILE *out = fopen("resultadosEnTiempos.csv", "w+");
	fprintf(out, "%s  %c %s %c  %s %c  %s", "Path imagen", ',', "Comienzo", ',', "Fin", ',', "Duracion\n");
	fclose(out);
	
	printf("\nIngrese el path e imagen\n");
	//scanf("%s", pathImagen); //Capturo la imagen
	strcpy(pathImagen1, "/home/lapassesungs/Escritorio/TP2/imagenes/imagen1.rgb");
	strcpy(pathImagen2, "/home/lapassesungs/Escritorio/TP2/imagenes/imagen2.rgb");
	strcpy(pathMask, "/home/lapassesungs/Escritorio/TP2/imagenes/mask.rgb");

	//Ejemplo:
    // /home/lapassesungs/Escritorio/TP2/imagenes/imagen1.jpg
	if(access(pathImagen1, F_OK) != 0) //Chequeo la existencia del file
	{
		printf("\nLa imagen %s no existe\n", pathImagen1);
		return 0;
	}

	printf("\e[1;1H\e[2J"); //Limpio la consola
	printf("La imagen ingresada %s es correcta\n", pathImagen1);
	
	comienzo = clock();
	FILE *imagen1 = fopen(pathImagen1, "rb");
	FILE *imagen2 = fopen(pathImagen2, "rb");
	FILE *mask = fopen(pathMask, "rb");

	//Llamada a la funcion ACA
	enmascarar_c(pathImagen1, pathImagen2, pathMask, 10*10);
	fin = clock();

	FILE *out2 = fopen("resultadosEnTiempos.csv", "a");
	int tiempo = fin-comienzo;
	
	fprintf(out2, "%s %c %Lf %c %Lf %c %Lf  %s", 
	pathImagen1, ',', (long double)comienzo/CLOCKS_PER_SEC, ',', (long double)fin/CLOCKS_PER_SEC, ',', (long double)tiempo/CLOCKS_PER_SEC, "\n");

	fclose(out2);
	
	//Imprime tiempo demorado
	printf("\nTiempo consumido: %ld \n", fin-comienzo);

	return 0;
}


void enmascarar_c(unsigned char *imagen1, unsigned char *imagen2, unsigned char *mask, int cant)
{
	//unsigned char * buffer1 = (unsigned char *) malloc(sizeof(unsigned char) * cant);

	//fread(buffer1, cant, sizeof(unsigned char), imagen1);

	//FILE *nuevaImagen = fopen ( "/home/lapassesungs/Escritorio/TP2/imagenes/nuevaImagen.rgb", "w" );
	FILE *nuevaImagen = fopen ( "/home/lapassesungs/Escritorio/TP2/imagenes/imagen1.jpg", "w" );

	for(int i=0; i<cant; i++)
	{
		if(mask[i] !=0) //O es negro, 255 es blanco
		{
			fwrite( &imagen1[i], sizeof(char), sizeof(imagen1[i]), nuevaImagen );
		}
		else
		{
			fwrite( &imagen2[i], sizeof(char), sizeof(imagen2[i]), nuevaImagen );
		}
		
	}
	fclose ( nuevaImagen );

	printf("\n asd \n");
}