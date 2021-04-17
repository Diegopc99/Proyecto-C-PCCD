#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

struct mensaje{
    long mtype;
    int id_nodo;
    int ticket;
};

int receive(int cola,int *id_nodo_origen,int *ticket);
int send(int cola,int id_nodo_destino,int id_nodo_origen,int ticket);

int mi_ticket = 0;
int mi_id;
int id_nodos_pend[100];
int num_pendientes = 0;
int N = 0;
int idColaRequest;
int idColaReply;
int quiero = 0;
int max_ticket = 0;

void* hiloReceptor(void* args){

    int id_nodo_origen = 0;
    int ticket_origen = 0;

    do{
        //recibimos los tickets de cada nodo
        receive(idColaRequest,&id_nodo_origen,&ticket_origen); // Al pasarlo como puntero por parametro podemos mantener los cambios
        
        if(max_ticket > ticket_origen){  //Obtenemos el ticket mayor de los 2 
            max_ticket = max_ticket;
        }else{
            max_ticket = ticket_origen;
        }

        //if(mi_ticket > ticket_origen){
        if(!quiero || (ticket_origen < mi_ticket) || ((ticket_origen == mi_ticket) && (id_nodo_origen < mi_id))){
            //enviamos un reply al nodo si el ticket es menor
            send(idColaReply,id_nodo_origen,mi_id,0);

        }else{
            id_nodos_pend[num_pendientes++] = id_nodo_origen;   
        }
        
    }while(1);

    pthread_exit(NULL);

    return args;
}

void inicializacion(int N,int *id_nodos){

    /////////////////// CREAMOS EL BUZON //////////////////////////

    key_t key = ftok("/home/diego",20);

    idColaRequest = msgget(key,IPC_CREAT|0777); // Si no existe la cola la crea

    if(idColaRequest == -1){
        printf("Error al generar la cola de mensajes.\n");
        exit(-1);
    }

    printf("ID cola REQUEST de mensajes del NODO %i: %i\n",mi_id,idColaRequest);

    key_t key2 = ftok("/home/diego",40);

    idColaReply = msgget(key2,IPC_CREAT|0777);

    if(idColaReply == -1){
        printf("Error al generar la cola de mensajes.\n");
        exit(-1);
    }

    printf("ID cola REPLY de mensajes del NODO %i: %i\n",mi_id,idColaReply);

    //////////////// CREAMOS EL HILO DEL PROCESO RECEPTOR ///////////////

    pthread_t hilo_receptor;
    pthread_create(&hilo_receptor,NULL,hiloReceptor,"");

    //////////////////////////////////////////////////////////////////////////

    //////////////////////// CALCULAMOS EL RESTO DE NODOS ///////////////////

    int valor_nodo = 1;
    int i = 0;

    for(i=0;i<N-1;i++){  //Guardamos los id de nodo menos el nuestro para luego poder reenviar a todos los nodos

        if(valor_nodo==mi_id){ // Si estamos en mi id de nodo lo saltamos
            valor_nodo++;
        }
            id_nodos[i] = valor_nodo++; // Almacenamos el id de nod en el array
        

    }

    /////////////////////////////////////////////////////////////////////////

    //sleep(15);

    return;
}

int send(int cola,int id_nodo_destino,int id_nodo_origen,int ticket){
        
    struct mensaje mensaje;

    mensaje.mtype = id_nodo_destino; // Para que asi el destinatario pueda recojer sus mensajes
    mensaje.id_nodo = id_nodo_origen;
    mensaje.ticket = ticket;

    msgsnd(cola,(struct msgbuf *)&mensaje,sizeof(mensaje.id_nodo)*2,0);

    return 0;
}

int receive(int cola,int *id_nodo_origen,int *ticket){  //Utilizamos punteros para poder actualizar las variables con los datos recibidos

    struct mensaje mensaje;

    msgrcv(cola,(struct msgbuf *)&mensaje,sizeof(mensaje.id_nodo)*2,mi_id,0);

    *id_nodo_origen = mensaje.id_nodo;
    *ticket = mensaje.ticket;

    return 0;
}


int main(int args,char *argv[]){  // ./proceso-hilos mi_id nodos

    //pthread_t idHilo;

    mi_id = atoi(argv[1]);
    N = atoi(argv[2]);
    int id_nodos[N-1];
    int id_aux = 0;

    inicializacion(N,&id_nodos[0]);

    printf("%i\n",mi_id);
    printf("%i\n",id_nodos[0]);
    printf("%i\n",id_nodos[1]);
    printf("%i\n",id_nodos[2]);

    do{

        //srand(time(NULL));
        //mi_ticket = rand();

        int i = 0;

        quiero = 1;
        mi_ticket = max_ticket++; //Sustituimos el rand() por el maximo ticket

        for(i=0;i<N-1;i++){ // No metemos al primero del array porque es el propio nodo
            
            //enviamos a todos los nodos los tickets
            send(idColaRequest,id_nodos[i],mi_id,mi_ticket);
            printf("Solicitud enviada al nodo %i\n",id_nodos[i]);

        }

        for(i=0;i<N-1;i++){
            //recibimos los tickets de todos los nodos
            receive(idColaReply,&id_aux,&mi_ticket);
            printf("Solicitud recibida del nodo %i\n",id_aux);
        }

        ///////////////////SECCION CRITICA///////////////
        printf("ENTRANDO EN LA SECCION CRITICA ...\n");
            sleep(5);
        printf("SALIENDO DE LA SECCION CRITICA ...\n");

        ////////////////////////////////////////////////

        quiero = 0;

        for(i=0;i<num_pendientes;i++){
            //enviamos a cada nodo un reply de que hemos pasado la seccion critica
            send(idColaReply,id_nodos_pend[i],mi_id,0);
            printf("Respuesta enviada al nodo %i\n",id_nodos_pend[i]);
        }

        num_pendientes = 0;

    }while(1);

    return 0;
} 
