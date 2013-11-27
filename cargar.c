/*
 * Nombre: cargar.c
 * Contiene tres funciones para cargar elementos del árbol de distintos ficheros:
 * cargar()
 * cargar_ordenado()
 * contar()
 */

#include "arbol.h"




/*
 * Nombre: cargar_ordenado()
 * Descripción: Carga datos de el fichero "arbol.data" de números enteros ordenados.
 * Argumentos: 	Puntero a la raíz del árbol.
 * Valor Devuelto: Puntero a la raíz del árbol.
 * Alexander Gil Casas. 2013.
 */

Nodo* cargar_ordenado(Nodo* raiz)
{
    FILE *fp1;
    int n,d,i,err; // d= dato leido, n= numero de datos, i= auxiliar, f= auxiliar,err para buscar errores
    int *datos; // vector dinámico con los datos

    if(raiz!=NULL)
    {
        printf("\nEl árbol no esta vacío.");
        return raiz;
    }

    fp1=fopen("arbol.data","r");
    if(fp1==NULL)
    {
        printf("\n\nError en la apertura del fichero.");
        return(raiz);
    }
    i=0;
    n=0;
    while(feof(fp1)==0)
    {

        err=fscanf(fp1,"%d",&d);
        if(err==0)
        {
            printf("\nNo se pudo leer el fichero, contiene otro tipo de datos.\n");
            return raiz;
        }
        n++;
        if(d<=i)
        {
            printf("\nEl fichero no está ordenado.");
            return raiz;
        }
        i=d-1;
    }
    n=n-1;
    if(n==0)
    {
        printf("\nEl fichero no tiene datos.");
        return raiz;
    }
    fclose(fp1);

    datos=(int*)malloc(sizeof(int)*n);

    if(datos==NULL)
    {
        printf("\nNo se ha podido hacer la reserva de memoria.");
        return (raiz);
    }
    fp1=fopen("arbol.data","r");
    for(i=0; i<n; i++)
    {
        fscanf(fp1,"%d",&datos[i]);
    }

    raiz=insertar_ordenado(0,n,datos,raiz);
    for(i=0; i<n; i++)
        raiz=insertar_elemento(raiz,datos[i]);
    if(contar(raiz)!=n)
    {
        printf("\nError al leer el fichero.\n");
    }
    free(datos);
    fclose(fp1);
    return(raiz);
}


/*
 * Nombre: contar()
 * Descripción: Cuenta el número de elementos que posee el árbol
 * Argumentos: 	Puntero a la raíz del árbol.
 * Valor Devuelto: Número de elementos que posee el árbol.
 * Alexander Gil Casas. 2013.
 */

int contar(Nodo* raiz)
{
    if(raiz==NULL)
    {
        return 0;
    }
    return 1+contar(raiz->der)+contar(raiz->izq);
}

/*
 * Nombre: InsetarOrdenado()
 * Descripción: Inserta los datos de un fichero ordenado equilibrando el árbol
 * Argumentos: 	Inicio y final del vector de datos, vector de datos, raíz árbol
 * Valor Devuelto: Raíz al árbol.
 * Alexander Gil Casas. 2013.
 */
Nodo* insertar_ordenado(int inicio,int final,int *datos,Nodo* raiz)
{
    int mitad;
    mitad=(final-inicio)/2+inicio;
    if(final-inicio>2)
    {
        raiz=insertar_elemento(raiz,datos[mitad]);
        raiz=insertar_ordenado(inicio,mitad,datos,raiz);
        raiz=insertar_ordenado(mitad,final,datos,raiz);
    }
    return raiz;
}
