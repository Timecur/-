#include"../utili.h"
static void handle_connection(int sockfd)
{
    fd_set readfds;
    int retval = 0;
    char buffer[BUFFER_SIZE];
    int maxfd;
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        maxfd = sockfd;

        retval = select(maxfd+1, &readfds, NULL, NULL, NULL);
        if(retval == -1)
        {
            perror("select");
            return;
        }   

        if(FD_ISSET(sockfd, &readfds))
        {
            recv(sockfd, buffer, BUFFER_SIZE, 0); 
            printf("client recv self msg:> %s\n",buffer);
            //sleep(1);
            printf("Msg:>");
            scanf("%s",buffer);
            send(sockfd, buffer, strlen(buffer)+1, 0); 
        }   
    }   
    }
    int main(int argc, char *argv[])
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addrSer;
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(SERVER_PORT);
        addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);
        int retval = connect(sockfd, (struct sockaddr*)&addrSer, sizeof(struct sockaddr));
        if(retval == -1)
        {
            perror("connect");
            return -1;
    }
    else
    {
        printf("Client Connect Server OK.\n");
    }

        send(sockfd, "hello server.", strlen("hello server")+1, 0);
        handle_connection(sockfd);
        return 0;
    }
