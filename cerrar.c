
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
#define MUTEX "/semMUTEX"
#define S1 "/semS1"



/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/






void cerrar_programa(int sig)
{
    int i;
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
    exit(0);
}


