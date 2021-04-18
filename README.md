# Proyecto-C-PCCD
Proyecto-C-PCCD


# Proyecto C PCCD - Tickets

Dos implementaciones del algoritmo:
1. 1 proceso por nodo, N nodos en el sistema distribuido
2. N procesos por nodo, M nodos en el sistema distribuido

## Implementación 1:

Compilamos:

gcc -Wall -pthread proceso-hilos.c -o proceso-hilos

Invocamos de la siguiente manera cada nodo:

./proceso-hilos id_nodo N_nodos_totales

## Implementación 2:

Compilamos de la misma manera:

gcc -Wall -pthread nprocesos-nodo.c -o nprocesos-nodo

Invocamos cada nodo del sistema con la siguiente sintaxis:

./nprocesos-nodo id_nodo N_nodos_totales num_procesos

## NOTAS

Para tener una visualización cómoda de los datos, se llama a un sleep de unos pocos segundos al salir de la SC de cada proceso.

La zona crítica real de un proceso es cuando aparece por terminal la línea "Proceso X entra de la SECCIÓN CRÍTICA" hasta la línea "Proceso X sale de la SECCIÓN CRÍTICA". El resto de print son orientativos de la ejecución correcta del algoritmo.
