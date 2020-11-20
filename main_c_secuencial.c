#include<stdio.h>																																																												
#include<stdlib.h>
#include<time.h>
#include<pthread.h>																																																									
    
    unsigned t_ini, t_fin;
    double segs;
    char nombre1[20], nombre2[20], mascara[20]; // Nombres de archivo
    int largo1, largo2, largoM; 		// Tamaño archivos
    char c1,c2,c3,aux='\00';			// Caracter color negro
    unsigned char *buffer1, *buffer2, *bufferM; // puntero a datos archivo 
    FILE *arch1, *arch2, *masc;			// puntero a archivos
    FILE *dest;

   
    void abrir_archivos();
    void calcular_tamanio();
    void guardar_en_buffer();
    void escribir_y_cerrar_archivos();
    void *enmascarar_p(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    int enmascarar_c(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    int enmascarar_asm(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    int enmascarar_threads(unsigned char *a, unsigned char *b,unsigned char *mask, int cant);
    
    
int main()
{

    printf("Hola empezando \n");

    abrir_archivos();  
    calcular_tamanio();  
    guardar_en_buffer();
 
    enmascarar_c(buffer1,buffer2,bufferM,largo1);
    enmascarar_asm(buffer1,buffer2,bufferM,largo1);
    enmascarar_threads(buffer1,buffer2,bufferM,largo1);

    escribir_y_cerrar_archivos();
   
    return 0;
}




void abrir_archivos(){

    arch1=fopen("b1.bmp","rb");  // apertura de archivo origen 1
    if(arch1 == NULL ) {
     printf("No fue posible abrir el archivo origen 1\n");
     exit(-1);
    } 
    arch2=fopen("b2.bmp","rb");  // apertura de archivo origen 2
    if(arch2 == NULL ) {
     printf("No fue posible abrir el archivo origen 2\n");
     exit(-1);
    }
    masc=fopen("bmasc.bmp","rb");  // apertura de archivo mascara
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
    printf("Tiempo usado: %.16g milisegundos\n\n", segs * 1000.0);
    return 0;
}


int enmascarar_asm(unsigned char *a, unsigned char *b,unsigned char *mask, int cant){
    printf("Dentro de la funcion enmascarar ASM !!\n");

}


int enmascarar_threads(unsigned char *a, unsigned char *b,unsigned char *mask, int cant){
 
   pthread_t thr1;
   pthread_t thr2;
   pthread_t thr3;

   int cant1, cant2;
   int l3;
   
   cant1 = cant/3;
   cant2= cant-(2*cant1);
   l3= aux*2;
   printf("Dentro de la funcion enmascarar THREADS !!\n\n");
   printf("Total de elementos o Bytes a leer: %d \n", cant);
   
   printf("Hilo 1 leera %d  elementos\n", aux);
   printf("Hilo 2 leera %d  elementos\n", aux);
   printf("Hilo 3 leera %d  elementos\n", aux2);


   pthread_create(&thr1, NULL, enmascarar_p1, NULL);
   pthread_create(&thr2, NULL, enmascarar_p2, NULL);
   pthread_create(&thr3, NULL, enmascarar_p3, NULL);

   pthread_join(thr1, NULL);
   pthread_join(thr2, NULL);
   pthread_join(thr3, NULL);

   pthread_exit(NULL);



}

void *enmascarar_p1(void){

    printf("Dentro de la funcion enmascarar p1 !!\n");

    for (int i=0; i<cant1; i++){  
       if (*(mask+i) != aux)
            *(a+i) = *(b+i);

    }

}


void *enmascarar_p2(void){

    printf("Dentro de la funcion enmascarar p2 !!\n");

    for (int i=0; i<cant1; i++){  
       if (*(mask+cant+i) != aux)
            *(a+cant+i) = *(b+cant+i);

    }

}

void *enmascarar_p3(void){

    printf("Dentro de la funcion enmascarar p3 !!\n");

    for (int i=0; i<cant2; i++){  
       if (*(mask+l3+i) != aux)
            *(a+l3+i) = *(b+l3+i);

    }

}

