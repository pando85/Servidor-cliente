/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifndef CABECERA_INCLUIDA
#define CABECERA_INCLUIDA
#include "arbol.h"
#endif

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>


#define PERMS 0600
#define MUTEX "/raro123"
#define S1 "/ouyea123"
/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


int main(int argc, char *argv[])
{
    int Q1,Q2,memo,max_clientes,*dato,i; // dato para insertar/buscar/borrar , err para buscar errores,Q1 cola 1,Q2 cola 2,max_clientes,longitud mensaje
    key_t llave1, llave2,keymemo; // llaves para la creacion de colas y memoria compartida
    sem_t *s1, *mutex; // punteros para identificador de los semaforos

    struct mensaje_peticion peticion;
    struct mensaje_respuesta respuesta;

    Nodo *raizarbol=NULL;// Árbol vacío
    int *vector_clientes;
    int num_clientes;

    num_clientes=0; // Inicializamos el numero de clientes
    esta_proceso_terminado= FALSE;
    // Comprobación si se ha introducido el número máximo de clientes
    if(argc!=2)
    {
        printf("\n¡Error! Debe introducir el nº maximo de clientes como argumento.\n\n");
        exit(-1);
    }
    max_clientes=atoi(argv[1]);
    if(max_clientes==0)
    {
        printf("\n¡Error! Debe introducir el nº maximo de clientes como argumento.\n\n");
        exit(-1);
    }



    // CREANDO LAS COLAS
    llave1=ftok("/bin",'3');
    if(llave1==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    llave2=ftok("/bin",'4');
    if(llave2==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    Q1 = msgget(llave1, PERMS | IPC_CREAT);
    if(Q1==-1)
    {
        printf("¡Error! Al crear colas fallo con errno = %d\n",errno);
        exit(-1);
    }
    Q2 = msgget(llave2, PERMS | IPC_CREAT);
    if(Q2==-1)
    {
        printf("¡Error! Al crear colas fallo con errno = %d\n",errno);
        exit(-1);
    }



    // Creo memoria compartida
    keymemo=ftok("/bin",'5');
    if(keymemo==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    memo=shmget(keymemo,sizeof(int),IPC_CREAT | PERMS);
    if(memo==-1)
    {
        printf("¡Error! No se pudo acceder a la memoria compartida, fallo con errno = %d\n",errno);
        exit(-1);
    }


    // Creo e inicializo semaforo mutex a 1
    mutex=sem_open(MUTEX, O_CREAT, PERMS, 1);
    if(mutex==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }

    // Creo e inicializo semaforo s1 a max_clientes
    s1=sem_open(S1,O_CREAT,PERMS,max_clientes);
    if(s1==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }


    //Vector con los pids de los clientes
    vector_clientes=malloc(max_clientes*sizeof(pid_t));



    // Cargar fichero
    raizarbol=CargarOrdenado(raizarbol);

    // Ctrl-C cierra_programa
    signal (SIGINT,cerrar_programa);

    // Dato apunta a la memoria compartida
    dato=shmat(memo,0,0);


    while(!esta_proceso_terminado)
    {
        msgrcv(Q1,&peticion,sizeof(int),0,0);
        respuesta.tipo=peticion.tipo;
        switch(peticion.codigo_operacion)
        {
            // ALTA
        case 0:
            vector_clientes[num_clientes]=peticion.tipo;
            num_clientes++;
            sem_wait(s1);
            respuesta.codigo_error=0;
            break;
            // INSERTAR
        case 1:
            sem_wait(mutex);
            raizarbol=InsertarElemento(raizarbol,*dato);
            sem_post(mutex);
            break;
            // BORRAR
        case 2:
            sem_wait(mutex);
            raizarbol=Borrar(raizarbol,*dato);
            sem_post(mutex);
            break;
            // BUSCAR
        case 3:
            sem_wait(mutex);
            if(Buscar(raizarbol,*dato)!=NULL)
            {
                // Dato encontrado
            }
            else
            {
                // Dato no encontrado
            }
            sem_post(mutex);
            break;
            // TERMINAR
        case 4:
            // si lo elimino deuvelvo 1
            if(baja(peticion.tipo,vector_clientes,max_clientes)==1)
            {
                num_clientes--;
                sem_post(s1);
            }
            else
            {
                respuesta.codigo_error=1;
            }
            break;



        }

        msgsnd(Q2, &respuesta, sizeof(int),0);

        printf("\n\n");
        Visualizar(raizarbol);
        printf("\n\n");


  }
        // Guardar fichero
    GuardarFichero(raizarbol);

    // Borrar colas
    msgctl(Q1, IPC_RMID, 0);
    msgctl(Q2, IPC_RMID, 0);



    // Cerrar clientes
    for(i=0; i<num_clientes; i++)
    {
        kill(vector_clientes[i],SIGINT);
    }

    // Liberar memoria
    free(vector_clientes);


    // Cerrar y borrar semaforos
    sem_close(mutex);
    sem_close(s1);
    sem_unlink(MUTEX);
    sem_unlink(S1);

    // Destruye la memoria compartida
    shmctl(memo, IPC_RMID, 0);


    return 0;
}



