#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main(int args,char *argv[]){  // ./inicializador numero_nodos

    int id_queue = 0;
    key_t key;
    
    int modif = atoi(argv[1]); 


    //for(i=0;i<atoi(argv[1]);i++){

        key = ftok("/home/diego",modif+2);

        id_queue = msgget(key,IPC_CREAT|0777);

        if(id_queue == -1){
            printf("Error al generar la cola de mensajes.\n");
            exit(-1);
        }
        printf("ID cola de mensajes de nodo %i: %i\n",modif,id_queue);
   // }


    return id_queue;
}
