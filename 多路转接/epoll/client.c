#include"../utili.h"
static void add_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
 }
 static void delete_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd; 
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);}static void modify_event(int epollfd, int fd, int state){
    struct epoll_event ev; 
    ev.events = state;
    ev.data.fd = fd; 
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);}static void do_read(int epollfd,  int fd, int sockfd, char *buf){
    int nread = read(fd, buf, BUFFER_SIZE);
    
    if(nread == -1) 
    {
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else
    {
        if(fd == STDIN_FILENO)
            add_event(epollfd, fd, EPOLLIN);
        else
        {
            delete_event(epollfd, fd, EPOLLIN);
            add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
        }
    }
    printf("Ser :>%s", buf);}static void do_write(int epollfd, int fd, int sockfd, char *buf){
    int nwrite = write(fd, buf, strlen(buf)+1);
    if(nwrite == -1)
    {
        perror("write");
        close(fd);
    }
    else
    {
        if(fd == STDOUT_FILENO)
        {
            delete_event(epollfd, fd, EPOLLOUT);
        }
        else
        {
            modify_event(epollfd, fd, EPOLLIN);
        }
    }    
    memset(buf, 0, BUFFER_SIZE);}static void handle_events(int epollfd, struct epoll_event *events, int num,
                            int sockfd, char *buf)
  {
    int i;
    int fd;
    for(i=0; i<num; ++i)
    {
        fd = events[i].data.fd;
        if(events[i].events & EPOLLIN)
            do_read(epollfd, fd, sockfd, buf);
        else if(events[i].events, fd, sockfd, buf)
            do_write(epollfd, fd, sockfd, buf);
    }
    
    }
    static void handle_connection(int sockfd)
    {
        struct epoll_event events[EPOLLEVENTS];
        int epollfd = epoll_create(FDSIZE);
        add_event(epollfd, STDIN_FILENO, EPOLLIN);

        int ret;
        char buffer[BUFFER_SIZE];
        for(;;)
        {
            ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
            handle_events(epollfd, events, ret, sockfd, buffer);
    }    
    close(epollfd);}int main(void)
   {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addrSer;
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(SERVER_PORT);
        addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);
        connect(sockfd, (struct sockaddr*)&addrSer, sizeof(struct sockaddr));

    handle_connection(sockfd);
    close(sockfd);
    return 0;
   }
