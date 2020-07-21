# Multiplicación de matrices usando threads

Thread es un mecanismo por el cual un programa puede hacer mas de una cosa al mismo tiempo.
Como los procesos, los threads parecen correr simultaneamente; pero debemos de tener el concepto de que el
sistema corre una tarea a la vez, y esto lo administra el Scheduler.

Podemos decir entonces que un programa que crea un thread tendrá a partir de ese momento dos “time-slot's” en
la cola de ejecución del scheduller. Pero estos dos timeslots's están ejecutando un mismo programa, muy
probablemente en puntos de ejecución distintos y comparten el mismo espacio de memoria, descriptores de
archivos y otros recursos del sistema.

Esta característica de compartir espacio de memoria y descriptores de archivos facilita la comunicación entre los
threads y el programa que los creó. Es decir la problemática de ICP (InterProcess Communication) está resuelta.
Pero nada podemos asegurar en cuanto a la ejecución. Mucho menos que estos guardén alguna relación en los
tiempos de ejecució. Estos temas son privados de Scheduller. No porque un programa creó un thread podemos
asegurar que siempre se ejecutará el programa padre antes que el thread.
