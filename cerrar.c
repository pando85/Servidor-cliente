
/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/

#ifndef CABECERA_INCLUIDA
#define CABECERA_INCLUIDA
#include "arbol.h"
#endif

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
    printf("Guardando datos del arbol en el fichero...\n");
    // Guardar fichero
    GuardarFichero(raizarbol);



    printf("Cerrando clientes...\n");
    // Cerrar clientes
    for(i=0; i<num_clientes; i++)
    {
        kill(vector_clientes[i],SIGINT);
    }

    // Liberar memoria
    free(vector_clientes);

    printf("Cerrando colas...\n");
    // Borrar colas
    msgctl(Q1, IPC_RMID, 0);
    msgctl(Q2, IPC_RMID, 0);


    printf("Cerrando semaforos...\n");
    // Cerrar y borrar semaforos
    sem_close(mutex);
    sem_close(s1);
    sem_unlink(MUTEX);
    sem_unlink(S1);

    printf("Cerrando memoria compartida...\n");
    // Destruye la memoria compartida
    shmctl(memo, IPC_RMID, 0);
    exit(0);
}


