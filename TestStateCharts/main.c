/** \brief Módulo único, prototipo de lo que será la biblioteca para el manejo e implementación
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
            FSM_DispatchEvent(&customFSM,eEvta,NULL);
            break;
        case 'b':
            FSM_DispatchEvent(&customFSM,eEvtb,NULL);
            break;
        case 'c':
            FSM_DispatchEvent(&customFSM,eEvtc,NULL);
            break;
        case 'd':
            FSM_DispatchEvent(&customFSM,eEvtd,NULL);
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
        FSM_DispatchEvent(&customFSM,eEvtTick10ms,NULL);
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
    FSM_DispatchEvent(&customFSM, eEvtb, NULL);

    puts("---evento a");
    FSM_DispatchEvent(&customFSM, eEvta, NULL);

    puts("---evento b");
    FSM_DispatchEvent(&customFSM, eEvtb, NULL);

    puts("---evento c");
    FSM_DispatchEvent(&customFSM, eEvtc, NULL);

    puts("---evento d");
    FSM_DispatchEvent(&customFSM, eEvtd, NULL);




    return 0;
}
