/*
 * Nombre: control_clientes.c
 * Contiene las funciones necesarias para implementar el control de clientes activos en el servidor:
 * control_clientes()
 *
 */


#include "arbol.h"


/*
 * Nombre: controlclientes()
 * Descripción: Manda un mensaje para controlar la actividad de cada cliente( cada TIEMPO_ESPERA segundos), si el mensaje no es contestado
 * en el TIEMPO_RESPUESTA se llama a la función matar_cliente_inactivo.
 * Argumentos: 	Ninguno.
 * Valor Devuelto: Ninguno.
 * Alexander Gil Casas. 2013.
 */
void *control_clientes(void *parametro)
{
    int i;
    printf("Iniciando el control de clientes activos...\n");
    signal (SIGALRM,matar_cliente_inactivo);
    while(1)
    {
        for(i=0; i<num_clientes; i++)
        {
            if(vector_clientes[i]!=0)
            {
                matar_pid = vector_clientes[i];
                kill(vector_clientes[i],SIGUSR1);
                alarm(TIEMPO_RESPUESTA);
                printf("Comprobando actividad del cliente %d\n",matar_pid);
                msgrcv(Q_clientes_activos,&cliente_activo,sizeof(int),vector_clientes[i],0);
                alarm(0);
                sleep(TIEMPO_ESPERA);
            }
        }
    }
}

/*
 * Nombre: matar_cliente_inactivo()
 * Descripción: Hace uso de la función baja() para eliminar un cliente que no ha respondido a la petición del servidor.
 * Argumentos: 	Señal SIGALRM.
 * Valor Devuelto: Ninguno.
 * Alexander Gil Casas. 2013.
 */
void matar_cliente_inactivo(int sig)
{

    if(baja(matar_pid)==ELIMINADO)
    {
        sem_wait(sclientes);
        num_clientes--;
        sem_post(sclientes);
        printf("Eliminado el cliente %d por inactividad, clientes conectados %d/%d\n",matar_pid,num_clientes,max_clientes);
        sem_post(s1);
    }

}


