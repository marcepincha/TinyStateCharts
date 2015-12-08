#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "../Queue/queue.h"
#include "../TestStateCharts/TinyStateCharts.h"
#include "parserFSM.h"
#include "command.h"



#define QUEUE_NEW_CHAR_SIZE 4
#define QUEUE_TICK_10MS_SIZE 2
MACRO_CREATE_QUEUE(queue_NEW_CHAR,char,QUEUE_NEW_CHAR_SIZE);
MACRO_CREATE_QUEUE(queue_TICK_10MS,uint8_t,QUEUE_TICK_10MS_SIZE);

DWORD hilitoTimer (void* ppp);
DWORD hilitoTeclado ();

#define QUEUE_CMD_REQ_SIZE 2
MACRO_CREATE_QUEUE(queue_Commands,command_req_t,QUEUE_CMD_REQ_SIZE);

///parametros de la aplicacion
double REG01;

int main()
{


    parserFSM_t parserFSM;
    ParserFSM_init(&parserFSM);

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTimer,NULL,0,NULL);
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTeclado,NULL,0,NULL);

    for(;;)
    {
        char caracter;
        command_req_t comando;
        if(NULL != queue_read(&queue_NEW_CHAR,&caracter))
        {
            FSM_DispatchEvent((FSM_t*)&parserFSM,(evento_t)NEW_CHAR_SIG,&caracter);
        }
        if(NULL != queue_read(&queue_TICK_10MS,&caracter))
        {
            FSM_DispatchEvent((FSM_t*)&parserFSM,(evento_t)TICK_10MS_SIG,NULL);
        }
        if(NULL != queue_read(&queue_Commands,&comando))
        {
            DispatchCommand(&comando);
        }
    }

    printf("Hello world!\n");
    return 0;
}


DWORD hilitoTimer (void* ppp)
{
    do
    {
        uint8_t dummy = 0;
        Sleep(10);
        queue_write_FIFO(&queue_TICK_10MS,&dummy);
        //FSM_DispatchEvent(&customFSM,(evento_t)TICK_10MS_SIG,NULL);
    }
    while(1);
}





DWORD hilitoTeclado ()
{
    do
    {
        char param = getch();
        //FSM_DispatchEvent(&customFSM,(evento_t)NEW_CHAR_SIG,&param);
        queue_write_FIFO(&queue_NEW_CHAR,&param);
        //Sleep(50);
    }
    while(1);
}

