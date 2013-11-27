/*
 * Nombre: insertar.c
 * Contiene una función para insertar elementos en el árbol:
 * insertar_elemento()
 *
 */


#include "arbol.h"






/*
 * Nombre: insertar_elemento()
 * Descripción: Inserta un dato en el árbol
 * Argumentos: 	Puntero a la raíz del árbol, entero ha instertar
 * Valor Devuelto: Puntero a la hoja actual del árbol.
 * Alexander Gil Casas. 2013.
 */



Nodo* insertar_elemento(Nodo* raiz,int d)
{
    if (raiz==NULL) // Si no existe el nodo se crea y se inserta el dato
    {
        raiz=malloc(sizeof(Nodo));
        if (raiz!=NULL) // Se asigna el dato
        {
            raiz->dato = d;
            raiz->der= NULL;
            raiz->izq= NULL;
        }
        else    // Comprobamos si se ha asignado correctamente el dato
        {
            printf("\n\n\nNo se ha podido insertar el dato.\n");
        }
    }
    // Si existe el nodo se mueve recursivamente hasta el lugar correspondiente y que este vacío
    else
    {
        if(d>raiz->dato)
        {
            raiz->der = insertar_elemento(raiz->der,d);
        }
        else
        {
            if (d==raiz->dato)
            {
                return raiz;
            }
            raiz->izq = insertar_elemento(raiz->izq,d);
        }
    }
    return raiz;
}

