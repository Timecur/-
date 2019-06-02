#include "./utili.h"                                                                                                                                

void InputData(OperStruct *op)
{
    printf("Please input op1 and op2:");
    scanf("%d %d  ",&(op->op1), &(op->op2));
}

int main()
{   
    int cli_sock = socket(AF_INET, SOCK_STREAM, 0); 
    if(cli_sock == -1){
        perror("client creat sock failed!\n");
        exit(1);
    }   

    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t len = sizeof(struct sockaddr);
    int res = connect(cli_sock, (struct sockaddr*)&ser_addr, len);
    if(res == -1){
        perror("connect failed!\n");
 close(cli_sock);
        exit(2);
    }
    printf("connect sucessful!\n");

    char cmd[2];
    OperStruct op;
    int result;
    while(1){
        printf("Please input operator cmd:  ");
        scanf("%s", cmd);
        if(strcmp(cmd, "+") == 0){
            op.oper = ADD;
            InputData(&op);
        }
        else if(strcmp(cmd, "-") == 0){
            op.oper = SUB;
            InputData(&op);
        }
        else if(strcmp(cmd, "*") == 0){
            op.oper = MUL;
            InputData(&op);
        }
else if(strcmp(cmd, "/") == 0){
            op.oper = DIV;
            InputData(&op);
        }
        else if(strcmp(cmd, "QUIT") == 0){
            op.oper = QUIT;
        }
        else{
            printf("Cmd invalid\n");
        }
        
        res = send(cli_sock, &op, sizeof(op), 0);
        if(res == -1){
            printf("send failed!\n");
            continue;
        }
        if(op.oper == QUIT)
            break;
        res = recv(cli_sock, &result, sizeof(result), 0);
        if(res == -1){
            printf("recv failed!\n");
            continue;
        }
printf("result = %d\n", result);
    }       
            
    close(cli_sock);
    return 0;
}           
