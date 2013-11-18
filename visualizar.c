/*****************************************************************/
/* Nombre: visualizar.c                                                   */
/* Contiene una función para visualizar los elementos del árbol:          */
/* Visualizar()                                                           */
/*                                                                        */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "arbol.h"


/*****************************************************************/
/* Nombre: Visualizar()                                                   */
/* Descripción: Vislualiza toda la lista en orden.                        */
/* Argumentos: 	Puntero a la raíz del árbol                               */
/* Valor Devuelto: Puntero a la hoja actual del árbol.                    */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


Nodo* Visualizar(Nodo* raiz)
{
    if (raiz != NULL)
    {
        Visualizar(raiz->izq);
        printf(" %d ",raiz->dato);
        Visualizar(raiz->der);
    }
    return raiz;
}
