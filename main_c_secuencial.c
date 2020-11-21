#include<stdio.h>																																																												
#include<stdlib.h>
#include<string.h>
#include<time.h>
																																																									
    
    unsigned t_ini, t_fin;
    double t_c, t_asm;				// Tiempo ejecucion C y asm
    double segs;
    char nombre1[10], nombre2[10], mascara[10]; // Nombres de archivo
    int largo1, largo2, largoM; 		// Tamaño archivos
    char c1,c2,c3,aux='\00';			// Caracter color negro
    unsigned char *buffer1, *buffer2, *bufferM; // puntero a datos archivo 
    FILE *arch1, *arch2, *masc;			// puntero a archivos
    FILE *dest;					// archvivo de imagen de salida
    FILE *result;				// archivo CSV con estadisticas

   
    void abrir_archivos();
    void calcular_tamanio();
    void guardar_en_buffer();
    void escribir_y_cerrar_archivos();
    void metricas();
    int enmascarar_c(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    int enmascarar_asm(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
   
    
int main(int argc, char *argv[])
{
    printf("Hola empezando \n");

    strcat(nombre1,argv[1]);
    strcat(nombre2,argv[2]);
    strcat(mascara,argv[3]);


    printf("Nombres de archivos: \n");

    printf(" %s\n", nombre1);
    printf(" %s\n", nombre2);
    printf(" %s\n", mascara);

    abrir_archivos();  
    calcular_tamanio();  
    guardar_en_buffer();
 
    enmascarar_c(buffer1,buffer2,bufferM,largo1);

    t_ini = clock();
    enmascarar_asm(buffer1,buffer2,bufferM,largo1);
    t_fin = clock();
    segs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    t_asm = segs,
    printf("Tiempo usado en asm: %.16g milisegundos\n\n", segs * 1000.0);

    escribir_y_cerrar_archivos();
    metricas();
   
    return 0;
}




void abrir_archivos(){


    arch1=fopen(nombre1,"rb");  // apertura de archivo origen 1
    if(arch1 == NULL ) {
     printf("No fue posible abrir el archivo origen 1\n");
     exit(-1);
    } 

    arch2=fopen(nombre2,"rb");  // apertura de archivo origen 1
    if(arch2 == NULL ) {
     printf("No fue posible abrir el archivo origen 2\n");
     exit(-1);
    }

    masc=fopen(mascara,"rb");  // apertura de archivo origen 1
    if(masc == NULL ) {
     printf("No fue posible abrir el archivo origen masc\n");
     exit(-1);
    }
    dest=fopen("dest.bmp","wb");  //apertura de archivo final o destino
    if(dest == NULL ) {
     printf("No fue posible abrir el archivo destino\n");
     exit(-1);
    }
}

void calcular_tamanio(){

    fseek(arch1, 0L, SEEK_END);   //me posiciono al final del archivo1
    largo1 = ftell(arch1);        //guardo tamaño de archivo 1
    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio
    
    fseek(arch2, 0L, SEEK_END);   //me posiciono al final del archivo2
    largo2 = ftell(arch2);        //guardo tamaño de archivo 2
    fseek(arch2, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    fseek(masc, 0L, SEEK_END);    //me posiciono al final del archivo mascara
    largoM = ftell(masc);         //guardo tamaño de la mascara
    fseek(masc, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    if (largo1 != largo2 || largo1 != largoM){ //Verifico que sean del mismo tamaño
        printf(" Archivos de distinto tamaño\n");
        exit(-1);
    }
}

void guardar_en_buffer(){

    buffer1 = (unsigned char *) malloc(sizeof(unsigned char) * largo1);
    fread(buffer1, largo1, sizeof(unsigned char), arch1);
																																																																																																																																																																																																																												    	buffer2 = (unsigned char *) malloc(sizeof(unsigned char) * largo2);
    fread(buffer2, largo2, sizeof(unsigned char), arch2);

    bufferM = (unsigned char *) malloc(sizeof(unsigned char) * largoM);
    fread(bufferM, largoM, sizeof(unsigned char), masc);
}

void escribir_y_cerrar_archivos(){

    fwrite(buffer1, largo1, sizeof(unsigned char), dest);   
    fclose(arch1);
    fclose(arch2);
    fclose(masc);
    fclose(dest);

    free(buffer1);
    free(buffer2);
    free(bufferM);
    
}


int enmascarar_c(unsigned char *a, unsigned char *b,unsigned char *mask, int cant){

    printf("Dentro de la funcion enmascarar C !!\n");
    t_ini = clock();
    for (int i=0; i<cant; i++){  //tantas iteraciones como bytes tiene el archivo
       if (*(mask+i) != aux)
            *(a+i) = *(b+i);

    }
    t_fin = clock();
    
    segs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    t_c = segs,
    printf("Tiempo usado en C: %.16g milisegundos\n\n", segs * 1000.0);
    return 0;
}


int enmascarar_asm(unsigned char *a, unsigned char *b,unsigned char *mask, int cant){
    printf("Dentro de la funcion enmascarar ASM !!\n");

}


void metricas(){

    char info[50], stamanio[10], stiempo_c[20], stiempo_asm[20];

    result=fopen("metricas.csv","a");  //apertura de archivo de metricas
    if(result == NULL ) {
     printf("No fue posible abrir el archivo de metricas\n");
     exit(-1);
    }	

    sprintf(stamanio, "%d", largo1);	//convierto a string el tamaño archivo
    sprintf(stiempo_c, "%.16g", t_c);  //convierto a string el tiempo
    sprintf(stiempo_asm, "%.16g", t_asm);  //convierto a string el tiempo


    strcat(info,nombre1);
    strcat(info,",");
    strcat(info,stamanio);
    strcat(info,",");
    strcat(info,stiempo_c );
    strcat(info,",");
    strcat(info,stiempo_asm);


    fputs(info, result);  
    fclose(result);
  
    printf("Contenido archivo metricas.csv: %s \n", info);

}
