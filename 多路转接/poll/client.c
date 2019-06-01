#include"../utili.h"
static void handle_connection(int sockfd)
{
    struct pollfd pfds[2];
    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;

    char buffer[BUFFER_SIZE];
    for(;;)
    {
        poll(pfds, 2, -1);  //-1表示永不超时
        if(pfds[0].revents & POLLIN)
        {
            recv(sockfd, buffer, BUFFER_SIZE, 0); 
            printf("msg:> %s\n",buffer);
        }   
        if(pfds[1].revents & POLLIN)
        {
            scanf("%s", buffer);
            //read(STDIN_FILENO, buffer, BUFFER_SIZE);
            send(sockfd, buffer, strlen(buffer)+1, 0); 
        }   
    }   
    }
    int main(void)
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        struct sockaddr_in addrSer;
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(SERVER_PORT);
        addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);
        connect(sockfd, (struct sockaddr*)&addrSer, sizeof(struct sockaddr));
    
        handle_connection(sockfd);
    return 0;
    }
