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
MACRO_CREATE_QUEUE(queue_NEW_CHAR_SERIE,char,QUEUE_NEW_CHAR_SIZE);
MACRO_CREATE_QUEUE(queue_TICK_10MS,uint8_t,QUEUE_TICK_10MS_SIZE);

DWORD hilitoTimer (void* ppp);
DWORD hilitoTeclado ();
DWORD hilitoSerialPort();

#define QUEUE_CMD_REQ_SIZE 2
MACRO_CREATE_QUEUE(queue_Commands,command_req_t,QUEUE_CMD_REQ_SIZE);

///parametros de la aplicacion
double REG01;
uint16_t SOLO_LECTURA = 1234;


///
void WriteSerial(char *arg);

int main()
{


    parserFSM_t parserFSM;
    ParserFSM_init(&parserFSM,(responseHandler_t)printf);

    parserFSM_t parserFSM_serie;
    ParserFSM_init(&parserFSM_serie,WriteSerial);

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTimer,NULL,0,NULL);
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoTeclado,NULL,0,NULL);
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hilitoSerialPort,NULL,0,NULL);

    for(;;)
    {
        char caracter;
        command_req_t comando;
        if(NULL != queue_read(&queue_NEW_CHAR,&caracter))
        {
            FSM_DispatchEvent((FSM_t*)&parserFSM,(evento_t)NEW_CHAR_SIG,&caracter);
        }
        if(NULL != queue_read(&queue_NEW_CHAR_SERIE,&caracter))
          {
              FSM_DispatchEvent((FSM_t*)&parserFSM_serie,(evento_t)NEW_CHAR_SIG,&caracter);
          }
        if(NULL != queue_read(&queue_TICK_10MS,&caracter))
        {
            FSM_DispatchEvent((FSM_t*)&parserFSM,(evento_t)TICK_10MS_SIG,NULL);
            FSM_DispatchEvent((FSM_t*)&parserFSM_serie,(evento_t)TICK_10MS_SIG,NULL);
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


#define STRING_SIZE 1
HANDLE hComPort;
DWORD hilitoSerialPort ()
{


    DWORD dwBytesWritten;
    DWORD dwBytesRead;

    char param;




    hComPort = CreateFile("COM1:",
                          GENERIC_READ | GENERIC_WRITE, // solo pueden ser estos valores
                          0,
                          0,
                          OPEN_EXISTING, // solo puede ser OPEN_EXISTING
                          FILE_ATTRIBUTE_NORMAL,
                          0);




    if (hComPort == INVALID_HANDLE_VALUE)
    {
        // no se pudo abrir el puerto COM1:
        puts("No se pudo abrir el puerto COM." );
        return 0;
    }


    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = MAXDWORD ;
    timeouts.ReadTotalTimeoutMultiplier = MAXDWORD ;
    timeouts.ReadTotalTimeoutConstant = 10;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 1;

    if (!SetCommTimeouts(hComPort, &timeouts))
        puts("setting port time-outs.");

    WriteFile(hComPort,"hola!",5,&dwBytesWritten,NULL);

    do
    {
        if(ReadFile(hComPort,&param, 1, &dwBytesRead, NULL) && dwBytesRead)
        {

                //printf("Se leyeron %d bytes. Buffer: %c\n" , dwBytesRead ,&param);

                //WriteFile(hComPort,&param,1,&dwBytesWritten,NULL);

                queue_write_FIFO(&queue_NEW_CHAR_SERIE,&param);
                //FSM_DispatchEvent((FSM_t*)&parserFSM_serie,(evento_t)NEW_CHAR_SIG,&param);

        }

        Sleep(50);
    }
    while(1);

    // finalmente, cerramos el manejador
    CloseHandle(hComPort);
}

void WriteSerial(char *arg)
{
    DWORD dwBytesWritten;
    //printf(arg);
    WriteFile(hComPort,arg,strlen(arg),&dwBytesWritten,NULL);
}
