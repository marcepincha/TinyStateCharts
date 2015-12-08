#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>

typedef struct estCommand command_t;
typedef enum eCmdType{write,read,exec} cmdType_t;
typedef void (*responseHandler_t)(char*);
typedef struct{command_t* command; cmdType_t type; char arg[20]; responseHandler_t responseHandler;} command_req_t;


typedef void (*cmdHandler_t) (command_req_t *cmd_req);      ///declaracion para tipo de dato de manejadores de comando
typedef void (*cmdWriteHandler_t) (command_req_t *cmd_req);
typedef void (*cmdReadHandler_t) (command_req_t *cmd_req);

///declaración de tipo de dato para comandos
typedef struct estCommand{
    char commandID[16];
    void *object;
    cmdWriteHandler_t write;
    cmdReadHandler_t read;
    cmdHandler_t excecute;
}command_t;

extern const command_t cmdTable[];
extern const uint8_t cmdTableSize;

void DispatchCommand(command_req_t* cmd_req);
command_req_t* CommandRequest_init(command_req_t *cmd_req, char* buffer, command_t *cmd_table);

#endif // COMMAND_H_
