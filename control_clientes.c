/*****************************************************************/
/* Nombre: main.c                                                         */
/* Contiene la función principal del programa con el menú:                */
/* main()                                                                 */
/*                                                                        */
/*****************************************************************/




#include "arbol.h"


/*****************************************************************/
/* Nombre: main()                                                         */
/* Descripción: Carga el menú principal.                                  */
/* Argumentos: 	Nº máximo de clientes.                                    */
/* Valor Devuelto: 0 si se ha ejecutado con éxito el programa.            */
/* Alexander Gil Casas. 2013.                                             */
/*****************************************************************/
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

void matar_cliente_inactivo(int sig)
{

    if(baja(matar_pid,vector_clientes,max_clientes)==ELIMINADO)
    {
        sem_wait(sclientes);
        num_clientes--;
        sem_post(sclientes);
        printf("Eliminado el cliente %d por inactividad, clientes conectados %d/%d\n",matar_pid,num_clientes,max_clientes);
        sem_post(s1);
    }

}


