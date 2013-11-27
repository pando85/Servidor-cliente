/*
 * Nombre: visualizar.c
 * Contiene una función para visualizar los elementos del árbol:
 * visualizar()
 *
 */


#include "arbol.h"



/*
 * Nombre: visualizar()
 * Descripción: Vislualiza toda la lista en orden.
 * Argumentos: 	Puntero a la raíz del árbol
 * Valor Devuelto: Puntero a la hoja actual del árbol.
 * Alexander Gil Casas. 2013.
 */


Nodo* visualizar(Nodo* raiz)
{
    if (raiz != NULL)
    {
        visualizar(raiz->izq);
        printf(" %d ",raiz->dato);
        visualizar(raiz->der);
    }
    return raiz;
}
