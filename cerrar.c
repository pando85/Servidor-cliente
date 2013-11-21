

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






void cerrar_programa(int sig)
{
    esta_proceso_terminado = TRUE;
    int i;
    key_t llave1, llave2,keymemo; // llaves para la creacion de colas y memoria compartida
    sem_t *s1, *mutex; // punteros para identificador de los semaforos
    int Q1,Q2;
    int memo;


    // ABRIENDO LAS COLAS
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
    Q1= msgget(llave1, 0);
    if(Q1==-1)
    {
        printf("¡Error! No esta ejecutado el servidor.\n");
        exit(-1);
    }
    Q2= msgget(llave2, 0);
    if(Q2==-1)
    {
        printf("¡Error! No esta ejecutado el servidor.\n");
        exit(-1);
    }

    // ABRIENDO LA MEMORIA COMPARTIDA
    keymemo=ftok("/bin",'5');
    if(keymemo==-1)
    {
        printf("¡Error! ftok fallo con errno = %d\n",errno);
        exit(-1);
    }
    memo=shmget(keymemo,sizeof(int),PERMS);
    if(memo==-1)
    {
        printf("¡Error! No se pudo acceder a la memoria compartida, fallo con errno = %d\n",errno);
        exit(-1);
    }
    // ABRIENDO LOS SEMAFOROS
    s1=sem_open(S1,  0);

    if(s1==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }

    mutex=sem_open(MUTEX,0);
    if(mutex==SEM_FAILED)
    {
        printf("¡Error! No se pudo abrir el semaforo, fallo con errno = %d\n",errno);
        exit(-1);
    }



    exit(0);
    return ;
}


