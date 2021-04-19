#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main(int args,char *argv[]){  // ./inicializador numero_nodos

    int id_queue = 0;
    int id_colas_nodos[args-1];
    key_t key;
    int i = 0;
    int seleccion = 0;

while(1){

    printf("1)Crear colas de mensajes\n");
    printf("2)Eliminar colas de mensajes\n");
    printf("3)Salir\n");

    scanf("%i",&seleccion);

    switch(seleccion){
        case 1:

            for(i=0;i<atoi(argv[1]);i++){

                key = ftok("/home/diego",i+20);

                id_queue = msgget(key,IPC_CREAT|0777);

                if(id_queue == -1){
                    printf("Error al generar la cola de mensajes.\n");
                    exit(-1);
                }
                id_colas_nodos[i] = id_queue;
                printf("ID cola de mensajes de nodo %i: %i\n",i,id_queue);
            }

            break;

        case 2:

            for(i=0;i<atoi(argv[1]);i++){

                int status = 0;

                status = msgctl(id_colas_nodos[i],IPC_RMID,NULL);

                if(status == -1){
                    printf("Error al eliminar la cola.\n");
                    exit(-1);
                }

                printf("Eliminada la cola con identificador: %i\n",id_colas_nodos[i]);

            }


            break;

        case 3:

            printf("Saliendo..\n");
            exit(0);

        default:

            printf("Opcion seleccionada incorrecta.\n");
            break;

    }
}

    return 0;
}
