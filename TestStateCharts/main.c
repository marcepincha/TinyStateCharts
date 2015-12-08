/** \brief M�dulo �nico, prototipo de lo que ser� la biblioteca para el manejo e implementaci�n
de StateCharts en C.
 *
 *  10/11/2015
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "TinyStateCharts.h"
#include "customFSM_events.h"



extern FSM_t customFSM;
void customFSM_iniciar();

DWORD hilitoTeclado ()
{
    char c;
    do
    {
        c=getchar();
        switch(c)
        {
        case 'a':
            FSM_DispatchEvent(&customFSM,(evento_t){eEvta},NULL);
            break;
        case 'b':
            FSM_DispatchEvent(&customFSM,(evento_t){eEvtb},NULL);
            break;
        case 'c':
            FSM_DispatchEvent(&customFSM,(evento_t){eEvtc},NULL);
            break;
        case 'd':
            FSM_DispatchEvent(&customFSM,(evento_t){eEvtd},NULL);
            break;
        case 'x':
            exit(0);
        default:
            break;
        }
    }
    while(1);
}

DWORD hilitoTimer (void* ppp)
{
    do{
        Sleep(10);
        FSM_DispatchEvent(&customFSM,(evento_t){eEvtTick10ms},NULL);
    }while(1);
}

int main()
{

    customFSM_iniciar();

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTeclado,NULL,0,NULL);
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTimer,NULL,0,NULL);

    do
    {
        Sleep(1000);
        FSM_Process(&customFSM);
    }
    while(1);



    puts("---evento b");
    FSM_DispatchEvent(&customFSM, (evento_t){eEvtb}, NULL);

    puts("---evento a");
    FSM_DispatchEvent(&customFSM, (evento_t){eEvta}, NULL);

    puts("---evento b");
    FSM_DispatchEvent(&customFSM, (evento_t){eEvtb}, NULL);

    puts("---evento c");
    FSM_DispatchEvent(&customFSM, (evento_t){eEvtc}, NULL);

    puts("---evento d");
    FSM_DispatchEvent(&customFSM, (evento_t){eEvtd}, NULL);




    return 0;
}
