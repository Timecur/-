#include "./utili.h"                                                                                                                                

int ProcessHandler(int sock_conn)
{
    OperStruct op; 
    int result;
    while(1){
        int res = recv(sock_conn, &op, sizeof(op), 0); 
        if(res == -1){
            printf("recv data failed!\n");
            continue;
        }   
        if(op.oper == ADD){result = op.op1+op.op2;}
        else if(op.oper == SUB){result = op.op1-op.op2;}
        else if(op.oper == MUL){result = op.op1*op.op2;}
        else if(op.oper == DIV){
            if(op.op2 == 0){ 
                perror("op2 val error\n");
                exit(3);
            }   
            result = op.op1/op.op2;
        }   
        else if(op.oper == QUIT){break;}

res = send(sock_conn, &result, sizeof(result), 0);
        if(res == -1){ 
            printf("send data failed!\n");
            continue;
        }
    }
    close (sock_conn);
}       
            
int main () 
{       
    int ser_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(ser_sock == -1){ 
        perror("socket creat failed!\n");
        exit(1);
    }       
                
    struct sockaddr_in ser_addr, cli_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
     
    socklen_t len = sizeof(struct sockaddr);
    int res = bind(ser_sock, (struct sockaddr*)&ser_addr, len);
    if(res == -1){
        perror("bind failed!");
        close(ser_sock);
        exit(2);
    }
    
    listen(ser_sock, LISTEN_QUEUE);
            
    int sock_conn;
    while(1){
        printf("Server wait for Client connect...\n");
    
        sock_conn = accept(ser_sock, (struct sockaddr*)&cli_addr, &len);
        if(sock_conn == -1){
            printf("this time accept failed!\n");
            continue;
        }
        else{
            printf("Client accept success!\n");
            printf("Client port is: %d\n", ntohs(cli_addr.sin_port));
            printf("Client addr is: %s\n", inet_ntoa(cli_addr.sin_addr));
  }   
    
        pid_t pid;
        pid = fork();
        if(pid == 0){
            close(ser_sock);
            ProcessHandler(sock_conn);
            exit(0);
        }
        else if(pid > 0){
            close(sock_conn);
            continue;
        }
        else{
            perror("creat child process failed!\n");
            continue;
        }   
    }       
    close(ser_sock);
    return 0;
}           
