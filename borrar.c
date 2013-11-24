/*****************************************************************/
/* Nombre: borrar.c                                                       */
/* Contiene una función para borrar elementos del árbol:                  */
/* Borrar()                                                               */
/*                                                                        */
/*****************************************************************/


#ifndef CABECERA_INCLUIDA
    #define CABECERA_INCLUIDA
    #include "arbol.h"
#endif

/*****************************************************************/
/* Nombre: Borrar()                                                       */
/* Descripción: Borra un elemento de la lista.                            */
/* Argumentos: 	Puntero a la raíz del árbol , dato a borrar.              */
/* Valor Devuelto: Puntero a la raíz del árbol.                           */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/


Nodo* Borrar(Nodo* raiz,int d)
{
    Nodo *paux1,*paux2,*borr,*padre;//paux1= dato a intercambiar, paux2= puntero auxiliar
    borr=Buscar(raiz,d); //Puntero al dato a borrar
    padre=BuscarPadre(raiz,d);
    if(borr!=NULL) // Si no existe el dato en el árbol no borramos nada
    {
        /********************************************BORRAR HOJA******************************************/
        if(borr->der==NULL && borr->izq==NULL)
        {
            if(padre==NULL) //Si es la raiz la borramos y devolvemos NULL
            {
                free(borr);
                return NULL;
            }
            else
            {
                if(padre->dato>d)
                {
                    padre->izq=NULL;
                }
                else
                {
                    padre->der=NULL;
                }
            }
        }
        else
        {
            /********************************************BORRAR RAMA (NO RAIZ)******************************************/
            if(padre!=NULL)
            {
                if(borr->izq!=NULL) // Si se puede bajar por la izquierda bajamos por la rama izquierda
                {
                    paux1=borr->izq;
                    paux2=paux1->der;
                    while(paux2!=NULL) // Buscamos ultimo término de la derecha por su rama izquierda
                    {
                        paux1=paux2;
                        paux2=paux1->der;
                    }
                    if(padre->dato>d) // Cambiamos padre->izq
                    {
                        paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                        padre->izq=paux1;

                        if(paux2->dato!=borr->dato) //Si el padre del dato cambiado y el dato borrado son distinto nodo
                        {
                            paux2->der=paux1->izq; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                            paux1->izq=borr->izq;

                        }
                        paux1->der=borr->der;
                    }
                    else // Cambiamos padre->der
                    {
                        paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                        padre->der=paux1;
                        if(paux2->dato!=borr->dato)
                        {
                            paux2->der=paux1->izq; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                            paux1->izq=borr->izq;

                        }

                        paux1->der=borr->der;
                    }
                }
                else //Si no se puede bajar por la rama izquierda bajamos por la derecha
                {
                    paux1=borr->der;
                    paux2=paux1->izq;
                    while(paux2!=NULL) // Buscamos ultimo término de la izquierda por su rama derecha
                    {
                        paux1=paux2;
                        paux2=paux1->izq;
                    }
                    if(padre->dato>d) // Hay que cambiar padre->izq
                    {
                        paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                        padre->izq=paux1;

                        if(paux2->dato!=borr->dato)
                        {
                            paux2->der=paux1->izq; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                            paux1->der=borr->der;

                        }

                        paux1->izq=borr->izq;
                    }
                    else // Hay que cambiar padre->der
                    {
                        paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                        padre->der=paux1;
                        if(paux2->dato!=borr->dato)
                        {
                            paux2->der=paux1->izq; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                            paux1->der=borr->der;

                        }
                        paux1->izq=borr->izq;
                    }
                }

            }
            /********************************************BORRAR RAMA (RAIZ)******************************************/
            else
            {
                if(borr->izq!=NULL) // Si se puede bajar por la izquierda bajamos por la rama izquierda
                {
                    paux1=borr->izq;
                    paux2=paux1->der;
                    while(paux2!=NULL) // Buscamos ultimo término de la derecha por su rama izquierda
                    {
                        paux1=paux2;
                        paux2=paux1->der;
                    }
                    paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                    if(paux2->dato!=borr->dato) //Si el padre del dato cambiado y el dato borrado son distinto nodo
                    {
                        paux2->der=paux1->izq; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                        paux1->izq=borr->izq;
                    }
                    paux1->der=borr->der;
                }
                else //Si no se puede bajar por la rama izquierda bajamos por la derecha
                {
                    paux1=borr->der;
                    paux2=paux1->izq;
                    while(paux2!=NULL) // Buscamos ultimo término de la izquierda por su rama derecha
                    {
                        paux1=paux2;
                        paux2=paux1->izq;
                    }
                    paux2=BuscarPadre(raiz,paux1->dato);//paux2=Padre del dato a intercambiar
                    if(paux2->dato!=borr->dato)
                    {
                        paux2->izq=paux1->der; // El padre del dato cambiado ya no tiene que apuntar al dato cambiado
                        paux1->der=borr->der;

                    }
                    paux1->izq=borr->izq;
                }
                free(borr);
                return (paux1); // Si la raiz cambia hay que devolverle el puntero a la nueva raiz
            }
        }
        free(borr);
        return raiz;
    }
    else
    {
        printf("\nEl dato no se encuentra en el árbol.\n");
        return raiz;
    }
}
