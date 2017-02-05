#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern double REG01;
extern uint16_t SOLO_LECTURA;

typedef struct estCommand command_t;
typedef enum eCmdType{write,read,exec} cmdType_t;
typedef struct{command_t* command; cmdType_t type; char arg[20]; void (*responseHandler)(char*);} command_req_t;


typedef void (*cmdHandler_t) (command_req_t *cmd_req);      ///declaracion para tipo de dato de manejadores de comando
typedef void (*cmdWriteHandler_t) (command_req_t *cmd_req);
typedef void (*cmdReadHandler_t) (command_req_t *cmd_req);

void WriteNotSupported (command_req_t *cmd_req);
void ReadNotSupported (command_req_t *cmd_req);
void ExecNotSupported (command_req_t *cmd_req);
void exitExec (command_req_t *cmd_req);
void lsExec (command_req_t *cmd_req);
void WriteDouble(command_req_t *cmd_req);
void ReadDouble(command_req_t *cmd_req);
void ReadUINT16 (command_req_t *cmd_req);

///declaración de tipo de dato para comandos
typedef struct estCommand{
    char commandID[16];
    void *object;
    cmdWriteHandler_t write;
    cmdReadHandler_t read;
    cmdHandler_t excecute;
}command_t;

const command_t cmdTable[] = {      {"ls",NULL,WriteNotSupported,ReadNotSupported,lsExec},
                                    {"exit",NULL,WriteNotSupported,ReadNotSupported,exitExec},
                                    {"REG01",&REG01,WriteDouble,ReadDouble,ExecNotSupported},
                                    {"SOLO_LECTURA",&SOLO_LECTURA,WriteNotSupported,ReadUINT16,ExecNotSupported},
};

const uint8_t cmdTableSize = sizeof(cmdTable)/sizeof(cmdTable[0]);

void WriteNotSupported (command_req_t *cmd_req)
{
    cmd_req->responseHandler("\nOperación de asignación no soportada\n");
}

void ReadNotSupported (command_req_t *cmd_req)
{
    cmd_req->responseHandler("\nOperación de lectura no soportada\n");
}

void ExecNotSupported (command_req_t *cmd_req)
{
    cmd_req->responseHandler("\nOperación de ejecución no soportada\n");
}

void exitExec (command_req_t *cmd_req)
{

   char *ptr;
   long ret = 0;


   if(cmd_req->arg != NULL) ret = strtol(cmd_req->arg, &ptr, 10);

    if(NULL != cmd_req->responseHandler)
    {
        cmd_req->responseHandler("\nsaliendo de la aplicación\n");
    }

    exit(ret);

}

void lsExec (command_req_t *cmd_req)
{
    //char *arg = cmd_req->arg;
    cmd_req->responseHandler("\nLista de comandos:\n");

    int i;
    char temp[32];
    for(i=0;i<sizeof(cmdTable)/sizeof(cmdTable[0]); i++)
    {
        sprintf(temp,"\t%s\t[%c%c%c]\n",cmdTable[i].commandID,cmdTable[i].write==WriteNotSupported?'-':'W',
                                                        cmdTable[i].read==ReadNotSupported?'-':'R',
                                                        cmdTable[i].excecute==ExecNotSupported?'-':'X');
        cmd_req->responseHandler(temp);
    }
}

void WriteDouble(command_req_t *cmd_req)
{
    char *end;
    double ret;
    command_t *com = cmd_req->command;
    char* arg = cmd_req->arg;
    char temp[32];

    ret = strtod(arg,&end);
    if(ret!=0.0 || (ret==0.0 && arg!=end))
    {
        *(double*)(com->object)=ret;
        sprintf(temp,"\n%s = %lf\n",com->commandID,*(double*)(com->object));
    }
    else
    {
        sprintf(temp,"\nERROR: No se pudo realizar la operación.\n");
    }
    cmd_req->responseHandler(temp);
}
void ReadDouble(command_req_t* cmd_req)
{
    char temp[32];
    command_t *com = cmd_req->command;
    sprintf(temp,"\n%s = %lf\n",com->commandID,*(double*)(com->object));
    cmd_req->responseHandler(temp);
}

void ReadUINT16(command_req_t* cmd_req)
{
    char temp[32];
    command_t *com = cmd_req->command;
    sprintf(temp,"\n%s = %u\n",com->commandID,*(uint16_t*)(com->object));
    cmd_req->responseHandler(temp);
}

void DispatchCommand(command_req_t* cmd_req)
{

    switch(cmd_req->type)
        {
        case read:
            cmd_req->command->read(cmd_req);
            break;
        case write:
            cmd_req->command->write(cmd_req);
            break;
        case exec:
            cmd_req->command->excecute(cmd_req);
            break;
        }

}

command_req_t* CommandRequest_init(command_req_t *cmd_req, char* buffer, command_t *cmd_table, uint16_t cmd_table_size)
{
    int8_t length;
    int8_t delimiter;
    cmdType_t type;

    ///determino el tipo de comando
    length=0;
    delimiter = -1;
    type = exec;

    while(buffer[length])
    {
        if(buffer[length]==' ' && delimiter<0)
        {
            type = exec;
            delimiter = length;
        }
        if(buffer[length]=='=' && delimiter<0)
        {
            type = write;
            delimiter = length;
        }
        if(buffer[length]=='?' && delimiter<0)
        {
            type = read;
            delimiter = length;
        }

        length++;
    }

    if(length==0 || delimiter == 0)
        return NULL;    //si el primer caracter es alguno de los delimitadores o si el string está vacío retorno

    if(delimiter > 0)
        buffer[delimiter]='\0';        //finalizo la primer parte del buffer con un nulo para poder buscar comando

    ///determino el comando
    int8_t i,elegido;
    elegido = -1;
    for(i=0;i< cmd_table_size && length;i++)
    {
        if(0 == strcmp(cmd_table[i].commandID,buffer))
        {
            elegido = i;
            break;
        }
    }


    if(elegido >= 0)    ///si encuentro el comando armo el request
    {
        cmd_req->command = (command_t*)&cmd_table[elegido];
        cmd_req->type = type;
        if(delimiter > 0) strcpy(cmd_req->arg,buffer+delimiter+1);

        return cmd_req;
    }
    else      ///no se encontró el comando
    {
        return NULL;
    }
}

