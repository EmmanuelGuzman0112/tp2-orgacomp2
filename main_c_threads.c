#include<stdio.h>																																																												

#include<stdlib.h>

#include<time.h>

#include<pthread.h>																																																									

    

    unsigned t_ini, t_fin;

    double segs;

    char nombre1[20], nombre2[20], mascara[20]; // Nombres de archivo

    int largoImagen1, largoImagen2, largoMask; 		// Tama?o archivos

    char aux='\00';			// Caracter color negro

    unsigned char *buffer1, *buffer2, *bufferM; // puntero a datos archivo 

    FILE *arch1, *arch2, *masc;			// puntero a archivos

    FILE *dest;



    int pixelPCadaHilo;

   

    pthread_t thr1;

    pthread_t thr2;

    pthread_t thr3;

   

    void abrir_archivos();

    void calcular_tamanio();

    void guardar_en_buffer();

    void escribir_y_cerrar_archivos();

    void *enmascarar_p1(void *parametro);

    void *enmascarar_p2(void *parametro);

    void *enmascarar_p3(void *parametro);

 

    

int main()

{



    printf("Hola empezando \n");



    abrir_archivos();  

    calcular_tamanio();  

    guardar_en_buffer();



    //Calculo la division de pixeles para cada hilo

    pixelPCadaHilo = largoImagen1/3;



    printf("Total de elementos o Bytes a leer: %d \n", largoImagen1);  



    t_ini = clock();

    pthread_create(&thr1, NULL, enmascarar_p1, NULL);

    pthread_create(&thr2, NULL, enmascarar_p2, NULL);

    pthread_create(&thr3, NULL, enmascarar_p3, NULL);



    pthread_join(thr1, NULL);

    pthread_join(thr2, NULL);

    pthread_join(thr3, NULL);



    



    t_fin = clock();    

    segs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    printf("Tiempo usado: %.16g milisegundos\n\n", segs * 1000.0);



    escribir_y_cerrar_archivos();



    pthread_exit(NULL);

   

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

    largoImagen1 = ftell(arch1);        //guardo tama?o de archivo 1

    fseek(arch1, 0L, SEEK_SET);   // me posiciono nuevamente al principio

    

    fseek(arch2, 0L, SEEK_END);   //me posiciono al final del archivo2

    largoImagen2 = ftell(arch2);        //guardo tama?o de archivo 2

    fseek(arch2, 0L, SEEK_SET);   // me posiciono nuevamente al principio

   

    fseek(masc, 0L, SEEK_END);    //me posiciono al final del archivo mascara

    largoMask = ftell(masc);         //guardo tama?o de la mascara

    fseek(masc, 0L, SEEK_SET);   // me posiciono nuevamente al principio

   

    if (largoImagen1 != largoImagen2 || largoImagen1 != largoMask){ //Verifico que sean del mismo tama?o

        printf(" Archivos de distinto tama?o\n");

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

}







void *enmascarar_p1(void *parametro){



    printf("Dentro de la funcion enmascarar p1 !!\n");

    printf("Voy a leer desde %d a %d pixel \n", pixelPCadaHilo * 0, pixelPCadaHilo * 1);



    for (int i= pixelPCadaHilo * 0; i< pixelPCadaHilo * 1; i++){  

       if( *(bufferM + i) != aux )

            *(buffer1 + i) = *(buffer2 + i);



    }

        printf("Finalizo bien p1 \n");

}





void *enmascarar_p2(void *parametro){



    printf("Dentro de la funcion enmascarar p2 !!\n");

    printf("Voy a leer desde %d a %d pixel \n", pixelPCadaHilo * 1, pixelPCadaHilo * 2);



    for (int i= pixelPCadaHilo * 1; i<pixelPCadaHilo * 2; i++){  

       if( *(bufferM + i) != aux)

            *(buffer1 + i) = *(buffer2 +i);



    }

        printf("Finalizo bien p2 \n");

}



void *enmascarar_p3(void *parametro){



    printf("Dentro de la funcion enmascarar p3 !!\n");

    printf("Voy a leer desde %d a %d pixel \n", pixelPCadaHilo * 2, pixelPCadaHilo * 3);



    for (int i= pixelPCadaHilo * 2; i< pixelPCadaHilo * 3; i++){          

       if( *(bufferM + i ) != aux)

            *(buffer1 + i) = *(buffer2 + i);



    }

    printf("Finalizo bien p3 \n");

}



