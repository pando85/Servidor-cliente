/*****************************************************************/
/* Nombre: guardar.c                                                      */
/* Contiene dos funciones para guardar el árbol en un fichero:            */
/* GuardarFichero()                                                       */
/* Guardar()                                                              */
/*****************************************************************/

#ifndef CABECERA_INCLUIDA
    #define CABECERA_INCLUIDA
    #include "arbol.h"
#endif



/*****************************************************************/
/* Nombre: GuardarFichero()                                                */
/* Descripción: Crea un fichero y llama a la función guardar.              */
/* Argumentos: 	Puntero a la raíz del árbol.                               */
/* Valor Devuelto: Nada.                                                   */
/*****************************************************************/

void GuardarFichero(Nodo* raiz)
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
    Guardar(raiz,fp1);
    fclose(fp1);

    return;
}







/*****************************************************************/
/* Nombre: Guardar()                                                       */
/* Descripción: Guarda el árbol en inorden en un archivo de texto.          */
/* Argumentos: 	Puntero a la raíz del árbol , puntero al fichero.         */
/* Valor Devuelto: Puntero al fichero.                                    */
/*****************************************************************/

FILE* Guardar(Nodo* raiz,FILE *fp1)
{
    if(raiz!=NULL)
    {
        fp1=Guardar(raiz->izq,fp1);
        fprintf(fp1,"%d\n",raiz->dato);
        fp1=Guardar(raiz->der,fp1);
    }
    return fp1;
}






