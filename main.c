#include<stdio.h>																																																												
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>																																																								
    
    unsigned t_ini_c, t_fin_c;			// Capturan clocks en C
    unsigned t_ini, t_fin;			// Capturan clocks en threads
    unsigned t_ini_asm, t_fin_asm;		// Capturan clocks en ASM

    double t_c = 0, t_asm = 0, t_hilos = 0;				// Tiempo ejecucion C y asm
    double segs;
    double clocks_x_sec;
    char nombre1[10], nombre2[10], mascara[10]; // Nombres de archivo
    char ancho_alto[15]; 			// Nombres de archivo
    int largoImagen1, largoImagen2, largoM; 		// Tamaño archivos en n° de bytes
    char aux='\00';			// Caracter color negro
    unsigned char *bufferC, *buffer2, *bufferM; // puntero a datos archivo 
    unsigned char *bufferASM;			// puntero exclusivo para funcion ASM
    unsigned char *bufferC_hilos;			// puntero exclusivo para funcion C HILOS
    FILE *arch1, *arch2, *masc;			// puntero a archivos
    FILE *dest_c, *dest_asm, *dest_c_hilos;			// archvivo de imagen de salida
    FILE *result;				// archivo CSV con estadisticas

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
    int enmascarar_c(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    void enmascarar_asm();
    extern void enmascarar_asm_externa(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);


    
int main(int argc, char *argv[])
{

    clocks_x_sec = CLOCKS_PER_SEC;
    strcat(nombre1,argv[1]);		// guardo primer argumento (archivo 1)
    strcat(nombre2,argv[2]);
    strcat(mascara,argv[3]);
    strcat(ancho_alto,argv[4]);        // guardo ultimo argumento (largo ancho)	

    printf("Nombres de archivos:  \n");
    printf(" %s, ", nombre1);
    printf(" %s, ", nombre2);
    printf(" %s,", mascara);
    printf("    Ancho y alto: %s\n", ancho_alto);


    inicializar_metricas();
    abrir_archivos();  
    calcular_tamanio();  
    guardar_en_buffer();


    enmascarar_asm();
    enmascarar_c(bufferC,buffer2,bufferM,largoImagen1);
    enmascarar_threads();

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

    dest_c=fopen("_salida_c.bmp" ,"wb");  //apertura de archivo final o destino C
    if(dest_c == NULL ) {
     printf("No fue posible abrir el archivo destino C\n");
     exit(-1);
    }

    dest_asm=fopen("_salida_asm.bmp" ,"wb");  //apertura de archivo final o destino ASM
    if(dest_asm == NULL ) {
     printf("No fue posible abrir el archivo destino ASM\n");
     exit(-1);
    }

    dest_c_hilos=fopen("_salida_c_hilos.bmp" ,"wb");  //apertura de archivo final o destino C_HILOS
    if(dest_asm == NULL ) {
     printf("No fue posible abrir el archivo destino C hilos\n");
     exit(-1);
    }
}

void calcular_tamanio(){

    fseek(arch1, 0L, SEEK_END);   //me posiciono al final del archivo1
    largoImagen1 = ftell(arch1);        //guardo tamaño de archivo 1
    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    fseek(arch2, 0L, SEEK_END);   //me posiciono al final del archivo2
    largoImagen2 = ftell(arch2);        //guardo tamaño de archivo 2
    fseek(arch2, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    fseek(masc, 0L, SEEK_END);    //me posiciono al final del archivo mascara
    largoM = ftell(masc);         //guardo tamaño de la mascara
    fseek(masc, 0L, SEEK_SET);   // me posiciono nuevamente al principio
   
    if (largoImagen1 != largoImagen2 || largoImagen1 != largoM){ //Verifico que sean del mismo tamaño
        printf(" Archivos de distinto tamaño\n");
        exit(-1);
    }

    printf("Cantidad de Bytes de cada archivo: %d \n", largoImagen1);
}

void guardar_en_buffer(){

    bufferC = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen1);
    fread(bufferC, largoImagen1, sizeof(unsigned char), arch1);
    buffer2 = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen2);
    fread(buffer2, largoImagen2, sizeof(unsigned char), arch2);

    bufferM = (unsigned char *) malloc(sizeof(unsigned char) * largoM);
    fread(bufferM, largoM, sizeof(unsigned char), masc);

    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio
    bufferASM = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen1);
    fread(bufferASM, largoImagen1, sizeof(unsigned char), arch1);

    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio
    bufferC_hilos = (unsigned char *) malloc(sizeof(unsigned char) * largoImagen1);
    fread(bufferC_hilos, largoImagen1, sizeof(unsigned char), arch1);
}

void escribir_y_cerrar_archivos(){

    fwrite(bufferC, largoImagen1, sizeof(unsigned char), dest_c);  
    fwrite(bufferASM, largoImagen1, sizeof(unsigned char), dest_asm); 
    fwrite(bufferC_hilos, largoImagen1, sizeof(unsigned char), dest_c_hilos); 
    fclose(arch1);
    fclose(arch2);
    fclose(masc);
    fclose(dest_c);
    fclose(dest_asm);
    fclose(dest_c_hilos);

    free(bufferC);
    free(bufferASM);
    free(bufferC_hilos);
    free(buffer2);
    free(bufferM); 
}


int enmascarar_c(unsigned char *a, unsigned char *b,unsigned char *mask, int cant){

    printf("Ejecutando la funcion enmascarar C \n"); 
    t_ini_c = clock();
    for (int i=0; i<cant; i++){  //tantas iteraciones como bytes tiene el archivo
       if (*(mask+i) != aux)
            *(a+i) = *(b+i);

    }
    t_fin_c = clock();
    
    return 0;
}

void enmascarar_asm()
{
    printf("Ejecutando la funcion enmascarar ASM \n");

    t_ini_asm = clock(); 
    enmascarar_asm_externa(bufferASM,buffer2,bufferM,largoImagen1);
    t_fin_asm = clock();

}

/*HILOS */
void enmascarar_threads()
{
     //Calculo la division de pixeles para cada hilo
    int pixelPCadaHilo = largoImagen1/3;
    pthread_t thr1;
    pthread_t thr2;
    pthread_t thr3;

    printf("Ejecutando la funcion Hilos \n"																																																																																																																																																																																																																																													);

    struct parametros parametroHilos;



    //Hilo 1
    parametroHilos.pixelDesde = pixelPCadaHilo * 0;
    parametroHilos.pixelHasta = pixelPCadaHilo * 1;
    pthread_create(&thr1, NULL, enmascarar_p, (void *) &parametroHilos);
  

    //Hilo 2
    parametroHilos.pixelDesde = pixelPCadaHilo * 1;
    parametroHilos.pixelHasta = pixelPCadaHilo * 2;
    pthread_create(&thr2, NULL, enmascarar_p, (void *) &parametroHilos);


    //Hilo 3
    parametroHilos.pixelDesde = pixelPCadaHilo * 2;
    //parametroHilos.pixelHasta = pixelPCadaHilo * 3;
	parametroHilos.pixelHasta = largoImagen1;
    pthread_create(&thr3, NULL, enmascarar_p, (void *) &parametroHilos);
        

    t_ini = clock();
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);
    pthread_join(thr3, NULL);  
    t_fin = clock();

}


void *enmascarar_p(void *parametro){

    struct parametros * p = ( struct parametros *) parametro; //Le paso los parametros
    int recorridoDesde = p -> pixelDesde;
    int recorridoHasta = p -> pixelHasta;

    for (int i= recorridoDesde; i< recorridoHasta; i++){  
       if( *(bufferM + i) != aux )
            *(bufferC_hilos + i) = *(buffer2 + i);

    }

}
/*Fin Hilos */


void inicializar_metricas(){
    if(access("metricas.csv", F_OK) != 0) //Chequeo la existencia del file
    {
        result = fopen("metricas.csv", "w+"); //Crea un fichero (o trunca el existente) para escritura, permitiendo también lectura del mismo.
        if(result == NULL ) {
            printf("No fue posible crear el archivo de metricas\n");
            exit(-1);
        }	

        fprintf(result, "%s %c %s %c %s %c %s %c %s %c %s", "Nombre", ',', "Tamaño", ',', "Bytes" , ',', "Tiempo en C (en seg)", ',', "Tiempo en ASM  (en seg)", ',', "Tiempo en C Hilos  (en seg) \n");
        fclose(result);
    }
    else
    {
        result = fopen("metricas.csv", "a"); //Abre el archivo para escribir al final del mismo, o lo crea en caso de no existir aún.
        if(result == NULL ) {
            printf("No fue posible crear el archivo de metricas\n");
            exit(-1);
        }	

        fclose(result);
    }
}

void metricas(){

    char info[50], stamanio[10], stiempo_c[20], stiempo_asm[20];



    result=fopen("metricas.csv","a");  //abrir un archivo para lectura y escritura, el fichero debe existir.
    if(result == NULL ) {
        printf("No fue posible abrir el archivo de metricas\n");
        exit(-1);
    }	

    segs = (double)(t_fin_asm - t_ini_asm) / clocks_x_sec;
    segs = (double)(t_fin - t_ini) / clocks_x_sec;
    segs = (double)(t_fin_c - t_ini_c) / clocks_x_sec;


    segs = (double)(t_fin_asm - t_ini_asm) / clocks_x_sec;
    t_asm = segs,
    printf("  Tiempo usado en ASM: %.16g milisegundos\n", segs * 1000.0);

    segs = (double)(t_fin - t_ini) / clocks_x_sec;
    t_hilos = segs;
    printf("  Tiempo usado en Threads: %.16g milisegundos\n", t_hilos * 1000.0);

    segs = (double)(t_fin_c - t_ini_c) / clocks_x_sec;
    t_c = segs,
    printf("  Tiempo usado en C: %.16g milisegundos\n", segs * 1000.0);



    sprintf(stamanio, "%d", largoImagen1);	//convierto a string el tamaño archivo
    sprintf(stiempo_c, "%.16g", t_c);  //convierto a string el tiempo
    sprintf(stiempo_asm, "%.16g", t_asm);  //convierto a string el tiempo

    fprintf(result, "%s %c %s %c %d %c %s %c %Lf %c %Lf %s", nombre1, ',', ancho_alto, ',', largoImagen1, ',', stiempo_c, ',', (long double)t_asm, ',', (long double)t_hilos, "\n");
    fclose(result);

}

