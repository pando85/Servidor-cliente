/***********************************************************************/
/* Nombre: baja.c                                                              */
/* Contiene tres funciones para cargar elementos del árbol de distintos ficheros:*/
/* Cargar()                                                                      */
/* CargarOrdenado()                                                              */
/* Contar()                                                                      */
/***********************************************************************/

#ifndef CABECERA_INCLUIDA
    #define CABECERA_INCLUIDA
    #include "arbol.h"
#endif
/*****************************************************************/
/* Nombre: Cargar()                                                       */
/* Descripción: Carga datos de un fichero de números enteros.             */
/* Argumentos: 	Puntero a la raíz del árbol.                              */
/* Valor Devuelto: Puntero a la raíz del árbol.                           */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/

int baja(int pid,int *vector_clientes,int max_clientes)
{
    int i,encontrado=0;



    // Busco el cliente a eliminar
    for(i=0;i<max_clientes;i++)
    {
        if(vector_clientes[i]==pid)
        {
            encontrado=1;
            break;
        }
    }

    if(encontrado!=1)
    {
     return 0;
    }
    // Lo elimino y reordeno el vector_clientes para dejar el hueco libre al final

    for(i=i;i<max_clientes-1;i++)
    {
        vector_clientes[i]=vector_clientes[i+1];
    }


    // si todo ha salido bien devuelvo 1 y mando señal de terminación al proceso
    kill(pid,SIGINT);
    return 1;





}
