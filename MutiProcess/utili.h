#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define LISTEN_QUEUE 5 // listen第二个参数
#define BUFFER_SIZE 255
typedef enum{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    QUIT
}OPER_TYPE;

typedef struct OperStruct{
    int op1;
int op2;
    OPER_TYPE oper;
}OperStruct;
