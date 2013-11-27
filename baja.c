/*
 * Nombre: baja.c
 * Contiene la función para dar de baja a los clientes del servidor:
 * baja()
 *
 */




#include "arbol.h"


/*
 * Nombre: baja()
 * Descripción: Termina el proceso del cliente objetivo y lo elimina del vector clientes.
 * Argumentos: 	Pid del cliente a eliminar.
 * Valor Devuelto: ELIMINADO si se ha ejecutado con éxito el programa. NO_ELIMINADO si ha habido algún error.
 * Alexander Gil Casas. 2013.
 */
int baja(int pid)
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
        return NO_ELIMINADO;
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
