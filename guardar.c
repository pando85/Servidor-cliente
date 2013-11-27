/*
 * Nombre: guardar.c
 * Contiene dos funciones para guardar el árbol en un fichero:
 * guardar_fichero()
 * guardar()
 */

#include "arbol.h"



/*
 * Nombre: guardar_fichero()
 * Descripción: Crea el fichero "arbol.data" y llama a la función guardar.
 * Argumentos: 	Puntero a la raíz del árbol.
 * Valor Devuelto: Nada.
 */

void guardar_fichero(Nodo* raiz)
{
    FILE *fp1;
    if (raiz==NULL)
    {
        printf("\nEl árbol no tiene datos. ");
        return;
    }

    fp1=fopen("arbol.data","w+");
    if(fp1==NULL)
    {
        printf("\n\n\t\t­­Error en la apertura del fichero.");
        return;
    }
    guardar(raiz,fp1);
    fclose(fp1);

    return;
}







/*
 * Nombre: guardar()
 * Descripción: Guarda el árbol en inorden en un archivo de texto.
 * Argumentos: 	Puntero a la raíz del árbol , puntero al fichero.
 * Valor Devuelto: Puntero al fichero.
 */

FILE* guardar(Nodo* raiz,FILE *fp1)
{
    if(raiz!=NULL)
    {
        fp1=guardar(raiz->izq,fp1);
        fprintf(fp1,"%d\n",raiz->dato);
        fp1=guardar(raiz->der,fp1);
    }
    return fp1;
}






