#include"../utili.h"
typedef struct server_context_st
{
    int cli_cnt; //有多少个客户端
    int clifds[SIZE]; //客户端套接字集合
    fd_set allfds; //套接字集合
    int maxfd;     //套接字中最大的一个}server_context_st;static server_context_st *s_srv_ctx = NULL;static void server_uninit(){
    if(s_srv_ctx)
    {
        free(s_srv_ctx);
        s_srv_ctx = NULL;
    }   
    }
    
    static void server_init()
    {
        int i;
        s_srv_ctx = (server_context_st*)malloc(sizeof(server_context_st));
        assert(s_srv_ctx != NULL);
        memset(s_srv_ctx, 0, sizeof(server_context_st));
        for(i=0; i<SIZE; ++i)
        {
            s_srv_ctx->clifds[i] = -1; 
    }   
    }s
    tatic int create_server_proc(const char *ip, int port)
    {
        printf("ip>%s\n",ip);
        printf("port:>%d\n",port);
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addrSer;
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(port);
        addrSer.sin_addr.s_addr = inet_addr(ip);
        socklen_t len = sizeof(struct sockaddr);

        int yes = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        bind(fd, (struct sockaddr*)&addrSer, len);
        listen(fd, LISTEN_QUEUE);
        return fd;}static int accept_client_proc(int srvfd){
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(struct sockaddr);
        int clifd = accept(srvfd, (struct sockaddr*)&cliaddr, &len);

        printf("Server Accept Client Connect OK.\n");
        int i;
        for(i=0; i<SIZE; ++i)
        {
            if(s_srv_ctx->clifds[i] == -1)
            {
                s_srv_ctx->clifds[i] = clifd;
                s_srv_ctx->cli_cnt++;
                break;
        }
    }

    if(i == SIZE)
    {
        printf("too many client.\n");
    }
    }
    static void handle_client_msg(int fd, char *buf)
    {
        printf("recv buffer :>%s\n",buf);
        send(fd, buf, strlen(buf)+1, 0);}static void recv_client_msg(fd_set *readfds){
        int clifd;
        char buffer[BUFFER_SIZE];
        int i;

        for(i=0; i<=s_srv_ctx->cli_cnt; ++i)
        {
            clifd = s_srv_ctx->clifds[i];
            if(clifd < 0)
            {
                continue;
            }   
        if(FD_ISSET(clifd, readfds))
        {
            recv(clifd, buffer, BUFFER_SIZE, 0);
            handle_client_msg(clifd, buffer);
        }
    }
    }
        static void handle_client_proc(int srvfd)
        {
        int clifd = -1;
        fd_set *readfds = &s_srv_ctx->allfds;

        int retval;

        int i;
        struct timeval tv;
        while(1)
        {
            FD_ZERO(readfds);
            FD_SET(srvfd, readfds);
            s_srv_ctx->maxfd = srvfd;
            for(i=0; i<s_srv_ctx->cli_cnt; ++i)
            {
                clifd = s_srv_ctx->clifds[i];
            FD_SET(clifd, readfds);
            s_srv_ctx->maxfd = (clifd > s_srv_ctx->maxfd ? clifd : s_srv_ctx->maxfd);
        }     
        //retval  =select(maxfd+1, NULL, NULL, readfds)

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        retval = select(s_srv_ctx->maxfd+1, readfds, NULL, NULL, &tv);
        if(retval == -1){   //错误返回
            perror("select");
            return ;
        }
        if(retval == 0){   //处理超时
            printf("Server Wait Time Out.\n");
            continue;
        }

        if(FD_ISSET(srvfd, readfds)){
            accept_client_proc(srvfd); //处理客户端的连接
        }else{
            recv_client_msg(readfds); //服务器接收客户端的消息
        }

    }
    }
    
    int main(int argc, char *argv[])
    {
        server_init();
        int srvfd = create_server_proc(SERVER_IP, SERVER_PORT);
        handle_client_proc(srvfd);
        return 0;
        }
