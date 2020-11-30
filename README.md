# TP2 - OC2 - SIMD | Threads

## Descripción

Para la materia Orgenización del Computador II ,se pidió un programa que procese imagenes por medio de la linea de comandos. El procesamiento consiste en tomar dos imagenes y una mascara de selección, la cual se aplica para combinar las dos imagenes pasadas.
El presente repositorio contiene el código assembler y C que realiza dicha operación.


## Prerequisito
* Tener git instalado

## Instalación

* Traer el repositorio remoto a la pc a ejecutar el código:

```
git clone https://github.com/EmmanuelGuzman0112/tp2-orgacomp2/
```

## Resultados Obtenidos

Pasamos las siguientes dos imagenes y la mascara:

Imagen 1:

![img1](e1.bmp)


Imagen 2:

![img1](e2.bmp)


Máscara:

![masc](emasc.bmp)


Finalmente el resultado es:

![result](https://drive.google.com/uc?export=view&id=1ULZQj8d44otfczlJZiQ3H-LGOaPZ5KkY)

## Ejecución
* Para lograr lo anterior, ejecutar el script pasando las imagenes y la máscara:
 
```
sudo sh script.sh
```
Esto ejecutará un script que aplica la máscara a todas las imágenes.

## Metricas Obtenidas

INSERTAR TABLAS Y GRAFICOS

## Conclusión

Concluimos que gracias a los registros MXX, podemos reducir bastante el tiempo de ejecución con respecto a ejecutarlo de forma secuencial e incluso con respecto a la ejecución en hilos.
Al utilizar los registros implementados en MXX, estamos avanzando cada 8 bytes la imagen para su análisis, esta es la explicación a los tiempos con respecto al secuencial, por ejemplo, el cual, como su nombre lo dice: recorremos byte por byte cada uno de ellos en la imagen. Un comentario y no menor, los últimos bytes en la versión assembler los recorremos con registros de propósito general, esto seria de “forma secuencial” pero como comentarios arriba, solo son para los últimos 8 bytes.
Luego, en la versión de hilos, implementamos 3 hilos, que cada hilo recorre solo su porción de imagen, es decir, partimos en 3 la cantidad bytes de la imagen y lanzamos los tres hilos en paralelo, en este caso no notamos mucha mejoría con respecto al secuencial ya que consideramos el tiempo de creación de hilo y su lanzamiento para cada uno.


## Integrantes del Equipo

* **Emmanuel Guzmán** - [EmmanuelGuzman0112](https://github.com/EmmanuelGuzman0112)
* **Christian Lopez** 
* **Fabián Juárez** - [jfa320](https://github.com/jfa320)
