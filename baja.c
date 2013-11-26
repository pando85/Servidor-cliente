/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/




#include "arbol.h"


/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/
int baja(int pid,int *vector_clientes,int max_clientes)
{
    int i,encontrado=0;

    sem_wait(sclientes);
    // Busco el cliente a eliminar
    for(i=0; i<max_clientes; i++)
    {
        if(vector_clientes[i]==pid)
        {
            encontrado=1;
            break;
        }
    }

    if(encontrado!=1)
    {
        sem_post(sclientes);
        return 0;
    }
    // Lo elimino y reordeno el vector_clientes para dejar el hueco libre al final

    for(i=i; i<max_clientes-1; i++)
    {
        vector_clientes[i]=vector_clientes[i+1];
    }

    sem_post(sclientes);
    // si todo ha salido bien devuelvo 1 y mando señal de terminación al proceso
    kill(pid,SIGINT);
    return ELIMINADO;

}
