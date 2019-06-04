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
    return listenfd;
 }
 
 static void add_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    }
    static void delete_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
    
 }
 static void modify_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
 }
 
 static void handle_accept(int epollfd, int listenfd)
 {
    int clifd;
    struct sockaddr_in addrCli;
    socklen_t len = sizeof(struct sockaddr);
    clifd = accept(listenfd, (struct sockaddr*)&addrCli, &len);
    if(clifd != -1)
    {
        add_event(epollfd, clifd, EPOLLIN);
    }
  }
    static void do_read(int epollfd,  int fd, char *buf){
    int nread = read(fd, buf, BUFFER_SIZE);
    if(nread == -1)
    {
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else
    {
        printf("read msg:>%s\n",buf);
        modify_event(epollfd, fd, EPOLLOUT);
    }
    }
    static void do_write(int epollfd, int fd, char *buf){
    int nwrite = write(fd, buf, strlen(buf)+1);
    if(nwrite == -1)
    {
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    } 
    else
    {
        modify_event(epollfd, fd , EPOLLIN);
    }
    memset(buf, 0, BUFFER_SIZE);
    }
    static void handle_events(int epollfd, struct epoll_event *events, int num,
                            int listenfd, char *buf)
   {
        int i;
        int fd;
        for(i=0; i<num; ++i)
        {
            fd = events[i].data.fd;
            if((fd==listenfd) && (events[i].events&EPOLLIN)) //根据其结果分别进入三种状态
                handle_accept(epollfd, listenfd);  //申请与服务器连接
            else if(events[i].events & EPOLLIN)
                do_read(epollfd, fd, buf);  //只读
            else if(events[i].events & EPOLLOUT)        
            do_write(epollfd, fd, buf);  //只写
    }
  }
 static void do_epoll(int listenfd)
 {
    int ret;
    char buffer[BUFFER_SIZE];
    struct epoll_event events[EPOLLEVENTS];
    int epollfd = epoll_create(FDSIZE);
    add_event(epollfd, listenfd, EPOLLIN);
    for(;;)
   {
        //select poll
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd, buffer);
    }
    close(epollfd);}int main(void)
   {
        int listenfd;
        listenfd = socket_bind(SERVER_IP, SERVER_PORT);
        listen(listenfd, LISTEN_QUEUE);
        do_epoll(listenfd);
        return 0;
      }
