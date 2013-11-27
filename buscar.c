/*****************************************************************/
/* Nombre: buscar.c                                                       */
/* Contiene dos funciones para buscar elementos en el árbol:              */
/* Buscar()                                                               */
/* BuscarPadre()                                                          */
/*****************************************************************/

#include "arbol.h"


/*****************************************************************/
/* Nombre: buscar()                                                       */
/* Descripción: Busca un elemento por el árbol.                           */
/* Argumentos: 	Puntero a la raíz del árbol , dato a buscar.              */
/* Valor Devuelto: Puntero al nodo o NULL si no encuentra el dato         */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/

Nodo* buscar(Nodo* raiz,int d)
{
    if (raiz==NULL) // Si no encuentra el dato que devuelva NULL
    {
        return NULL;
    }
    else if (d<raiz->dato) // Si es menor busca por la izquierda
    {
        buscar(raiz->izq,d);
    }
    else if (d>raiz->dato) // Si es mayor busca por la derecha
    {
        buscar(raiz->der,d);
    }
    else                  // Si lo encuentra devuelve el nodo
    {
        return raiz;
    }
}


/*
 * Nombre: buscar_padre()
 * Descripción: Busca al padre de un elemento por el árbol.
 * Argumentos: Puntero a la raíz del árbol , dato a buscar.
 * Valor Devuelto:Puntero al padre del nodo o NULL si no encuentra el dato
 * Alexander Gil Casas. 2013.
 */



Nodo* buscar_padre(Nodo* raiz,int d)
{
    Nodo *paux2,*paux1;// paux2 =padre,paux1= puntero usado para buscar
    paux1=raiz;
    paux2=NULL;
    if(raiz==NULL)
        return paux2;
    while (paux1->dato!=d)
    {
        if(paux1->dato<d) //busco izquierda
        {
            paux2=paux1;
            paux1=paux1->der;
        }
        else
        {
            paux2=paux1;
            paux1=paux1->izq;
        }
        if (paux1==NULL)
        {
            return NULL;
        }
    }
    return paux2;
}
