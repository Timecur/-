#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<assert.h>
#include<sys/select.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT  8787
#define LISTEN_QUEUE 5
#define SIZE 10
#define BUFFER_SIZE 256
