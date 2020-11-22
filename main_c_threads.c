#include<stdio.h>																																																												
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<string.h>																																																			
    
    unsigned t_ini, t_fin;
    double t_c = 0, t_asm = 0, t_hilos = 0;				// Tiempo ejecucion C y asm
    double segs;
    char nombre1[20], nombre2[20], mascara[20]; // Nombres de archivo
    int largoImagen1, largoImagen2, largoMask; 		// Tama�o archivos
    char aux='\00';			// Caracter color negro
    unsigned char *buffer1, *buffer2, *bufferM; // puntero a datos archivo 
    FILE *arch1, *arch2, *masc;			// puntero a archivos
    FILE *dest;                 // archvivo de imagen de salida
    FILE *result;				// archivo CSV con estadisticas

    int pixelPCadaHilo;
   
    pthread_t thr1;
    pthread_t thr2;
    pthread_t thr3;

    struct parametros {
        int pixelDesde;
        int pixelHasta;
    };

    void inicializar_metricas();
    void abrir_archivos();
    void calcular_tamanio();
    void guardar_en_buffer();
    void escribir_y_cerrar_archivos();
    void enmascarar_threads();
    void *enmascarar_p(void *parametro); 
    void metricas();
    
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

    inicializar_metricas();
    abrir_archivos();  
    calcular_tamanio();  
    guardar_en_buffer();
    enmascarar_threads();
    escribir_y_cerrar_archivos();
    metricas();
    pthread_exit(NULL);
    
   
    return 0;
}




void abrir_archivos(){

    arch1=fopen(nombre1,"rb");  // apertura de archivo origen 1
    if(arch1 == NULL ) {
     printf("No fue posible abrir el archivo origen 1\n");
     exit(-1);
    } 
    arch2=fopen(nombre2,"rb");  // apertura de archivo origen 2
    if(arch2 == NULL ) {
     printf("No fue posible abrir el archivo origen 2\n");
     exit(-1);
    }
    masc=fopen(mascara,"rb");  // apertura de archivo mascara
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
    largoImagen1 = ftell(arch1);        //guardo tama�o de archivo 1
    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio
    
    fseek(arch2, 0L, SEEK_END);   //me posiciono al final del archivo2
    largoImagen2 = ftell(arch2);        //guardo tama�o de archivo 2
    fseek(arch2, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    fseek(masc, 0L, SEEK_END);    //me posiciono al final del archivo mascara
    largoMask = ftell(masc);         //guardo tama�o de la mascara
    fseek(masc, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    if (largoImagen1 != largoImagen2 || largoImagen1 != largoMask){ //Verifico que sean del mismo tama�o
        printf(" Archivos de distinto tama�o\n");
        exit(-1);
    }
}

void guardar_en_buffer(){

    buffer1 = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen1);
    fread(buffer1, largoImagen1, sizeof(unsigned char), arch1);

	buffer2 = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen2);
    fread(buffer2, largoImagen2, sizeof(unsigned char), arch2);

    bufferM = (unsigned char *) malloc(sizeof(unsigned char) * largoMask);
    fread(bufferM, largoMask, sizeof(unsigned char), masc);
}

void escribir_y_cerrar_archivos(){

    fwrite(buffer1, largoImagen1, sizeof(unsigned char), dest);   
    fclose(arch1);
    fclose(arch2);
    fclose(masc);
    fclose(dest);

    free(buffer1);
    free(buffer2);
    free(bufferM);
}

void enmascarar_threads()
{
     //Calculo la division de pixeles para cada hilo
    pixelPCadaHilo = largoImagen1/3;

    printf("Total de elementos o Bytes a leer: %d \n", largoImagen1);

    struct parametros parametroHilos;

    t_ini = clock();

    //Hilo 1
    parametroHilos.pixelDesde = pixelPCadaHilo * 0;
    parametroHilos.pixelHasta = pixelPCadaHilo * 1;
    pthread_create(&thr1, NULL, enmascarar_p, (void *) &parametroHilos);
    pthread_join(thr1, NULL);    

    //Hilo 2
    parametroHilos.pixelDesde = pixelPCadaHilo * 1;
    parametroHilos.pixelHasta = pixelPCadaHilo * 2;
    pthread_create(&thr2, NULL, enmascarar_p, (void *) &parametroHilos);
    pthread_join(thr2, NULL);

    //Hilo 3
    parametroHilos.pixelDesde = pixelPCadaHilo * 2;
    parametroHilos.pixelHasta = pixelPCadaHilo * 3;
    pthread_create(&thr3, NULL, enmascarar_p, (void *) &parametroHilos);
    pthread_join(thr3, NULL);        

    t_fin = clock();
    segs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    t_hilos = segs;
    printf("Tiempo usado: %.16g milisegundos\n\n", segs * 1000.0);
}


void *enmascarar_p(void *parametro){
    printf("Dentro de la funcion hilo !!\n");

    struct parametros * p = ( struct parametros *) parametro; //Le paso los parametros
    int recorridoDesde = p -> pixelDesde;
    int recorridoHasta = p -> pixelHasta;

    printf("Voy a leer desde %d a %d pixel \n", recorridoDesde, recorridoHasta);

    for (int i= recorridoDesde; i< recorridoHasta; i++){  
       if( *(bufferM + i) != aux )
            *(buffer1 + i) = *(buffer2 + i);

    }
        printf("Finalizo bien \n");
}

void inicializar_metricas(){
    result = fopen("metricas.csv", "w+"); //abrir un archivo para escritura, se crea si no existe o se sobreescribe si existe.
    if(result == NULL ) {
        printf("No fue posible crear el archivo de metricas\n");
        exit(-1);
    }	

	fprintf(result, "%s %c %s %c %s %c %s %c %s", "Nombre", ',', "Tamaño", ',', "Tiempo en C", ',', "Tiempo en ASM", ',', "Tiempo en C Hilos\n");
	fclose(result);

}

void metricas(){

    char info[50], stamanio[10], stiempo_c[20], stiempo_asm[20];

    result=fopen("metricas.csv","a");  //abrir un archivo para lectura y escritura, el fichero debe existir.
    if(result == NULL ) {
        printf("No fue posible abrir el archivo de metricas\n");
        exit(-1);
    }	

    sprintf(stamanio, "%d", largoImagen1);	//convierto a string el tamaño archivo
    sprintf(stiempo_c, "%.16g", t_c);  //convierto a string el tiempo
    sprintf(stiempo_asm, "%.16g", t_asm);  //convierto a string el tiempo


    strcat(info,nombre1);
    strcat(info,",");
    strcat(info,stamanio);
    strcat(info,",");
    strcat(info,stiempo_c );
    strcat(info,",");
    strcat(info,stiempo_asm);

    //fputs(info, result);
    //fprintf(result, "%s", "\n");
    fprintf(result, "%s %c %d %c %Lf %c %Lf %c %Lf %s", nombre1, ',', largoImagen1, ',', (long double)t_c, ',', (long double)t_asm, ',', (long double)t_hilos, "\n");
    fclose(result);
  
    printf("Contenido archivo metricas.csv: %s \n", info);

}