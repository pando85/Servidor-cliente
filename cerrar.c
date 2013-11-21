

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
}


