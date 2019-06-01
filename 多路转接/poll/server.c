#include"../utili.h"
static int socket_bind(const char *ip, int port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    //addrSer.sin_addr.s_addr = inet_addr(ip);
    inet_pton(AF_INET, ip, &addrSer.sin_addr);
    addrSer.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&addrSer, sizeof(struct sockaddr));
    return listenfd;}static void handle_connection(struct pollfd *connfds, int num){
    char buffer[BUFFER_SIZE];
    int i;
    for(i=1; i<=num; ++i)
    {
        if(connfds[i].fd == -1) 
            continue;
        if(connfds[i].revents & POLLIN){
            recv(connfds[i].fd, buffer, BUFFER_SIZE, 0); 
            printf("Server accept client msg:>%s\n",buffer);
            send(connfds[i].fd, buffer, strlen(buffer)+1, 0); 
        }   
    }   
    }
    static void do_poll(int listenfd)
    {
        struct sockaddr_in addrCli;
        int connfd;
        struct pollfd clientfds[OPEN_MAX];
        clientfds[0].fd = listenfd;
        clientfds[0].events = POLLIN;
        int i;
        for(i=1; i<OPEN_MAX; ++i)
        {
            clientfds[i].fd = -1;
    }

    int nready;
    int max = 0;
    for(;;)
    {
        nready = poll(clientfds, max+1, -1);
        if(nready == -1)
        {
            perror("poll");
            return;
        }
        if(nready == 0)
        {
            printf("Server Wait Time Out.\n");
            continue;
        }
        if(clientfds[0].revents & POLLIN)
        {  //收发消息
            socklen_t len = sizeof(struct sockaddr);
            connfd = accept(listenfd, (struct sockaddr*)&addrCli, &len);
            int i;
            for(i=1; i<OPEN_MAX; ++i)
            {
                if(clientfds[i].fd == -1)
                {
                    clientfds[i].fd = connfd;
                    break;
                }
            }

            if(i == OPEN_MAX)
            {
                printf("Over Load.\n");
                exit(1);
            }

            clientfds[i].events = POLLIN;
            max = (i>max ? i : max);
        }

        handle_connection(clientfds, max);  //建立连接
    }
    }
    int main(void)
    {
        int listenfd;
        listenfd = socket_bind(SERVER_IP, SERVER_PORT);
        listen(listenfd, LISTEN_QUEUE);
        do_poll(listenfd);
    return 0;
   }
