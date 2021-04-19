# Proyecto-C-PCCD
Proyecto-C-PCCD


# Proyecto C PCCD - Tickets

Sistema distribuido basado en solicitud / tickets, una cola por nodo, discriminando tipos de mensajes REPLY / REQUEST por el tipo mtype para cada cola.

Dos implementaciones del algoritmo:
1. 1 proceso por nodo, N nodos en el sistema distribuido
2. N procesos por nodo, M nodos en el sistema distribuido



Un proceso encargado de inicializar el sistema distribuido. Invocación:
> ./inicializador num_nodos

Se despliega un menú, que nos da a elegir entre crear o eliminar colas, o salir. Seleccionaremos la segunda opción cuando queramos finalizar la ejecución del sistema distribuido por completo. Seleccionamos la primera opción para crear las colas y nos devuelve el ID_cola del sistema operativo para cada nodo.

## Implementación 1:

Compilamos:

> gcc -Wall -pthread proceso-hilos.c -o proceso-hilos

Invocamos de la siguiente manera cada nodo:

> ./proceso-hilos id_nodo N_nodos_totales

## Implementación 2:

Compilamos de la misma manera:

> gcc -Wall -pthread nprocesos-nodo.c -o nprocesos-nodo

Invocamos cada nodo del sistema con la siguiente sintaxis:

> ./nprocesos-nodo id_cola_nodo_actual nodos_totales num_procesos_locales id_cola2 id_cola3 ... id_colaN

Siendo N el número total de nodos partícipes en el sistema distribuido, y pasando por línea de argumentos el id_cola de cada uno de los otros nodos (salvo el nuestro)

## NOTAS

Para tener una visualización cómoda de los datos, se llama a un sleep de unos pocos segundos al salir de la SC de cada proceso.

La zona crítica real de un proceso es cuando aparece por terminal la línea "Proceso X entra de la SECCIÓN CRÍTICA" hasta la línea "Proceso X sale de la SECCIÓN CRÍTICA". El resto de print son orientativos de la ejecución correcta del algoritmo.
 
