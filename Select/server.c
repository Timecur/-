#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化文件描述符集状态
void Init(int* fd_list, int fd_list_size){
  int i;
  for(i = 0; i < fd_list_size; i++){
    fd_list[i] = -1; 
  }
}

void Reload(int listen_fd, int* connect_list, int connect_list_size,\
    fd_set* read_fds, int* max_fd){
  // 清空文件描述符集
  FD_ZERO(read_fds);
  // 添加文件描述符到给定集合中
  FD_SET(listen_fd, read_fds);
  int max = listen_fd;
  int i;
  for(i = 0; i < connect_list_size; ++i){
    // 文件描述符不为空
    if(connect_list[i] != -1){
      // 添加文件描述符到指定集合
      FD_SET(connect_list[i], read_fds);
      // 记录文件描述符最大值
      if(connect_list[i] > max){
        max = connect_list[i];
      }
    } 
  }
  *max_fd = max;
}

void Add(int fd, int* connect_list, int connect_list_size){
  // 添加文件描述符到集合
  // 遍历集合中空闲的位置给新fd
  int i;
  for(i = 0; i < connect_list_size; i++){
    if(connect_list[i] == -1){
      connect_list[i] = fd;
      break;
    }
  }
  return;
}



int main(int argc, char* argv[]){
  if(argc != 3){
    printf("usage: ./server [ip] [port]\n");
    return 1;
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_fd < 0){
    perror("socket");
    return 1;
  }
  
  int ret = bind(listen_fd, (struct sockaddr*) &addr, sizeof(addr));
  if(ret < 0){
    perror("bind");
    return 1;
  }
  
  ret = listen(listen_fd, 5);
  if(ret < 0){
    perror("listen");
    return 1;
  }

  // 创建文件描述符集
  fd_set read_fds;
  int fd_list[1024];
  Init(fd_list, sizeof(fd_list)/sizeof(int));

  for(;;){
    int max_fd = listen_fd;
    
    // 每开始新的循环，重新设置文件描述符集
    Reload(listen_fd, fd_list, sizeof(fd_list)/sizeof(int), &read_fds, &max_fd);
    printf("before select:%d\n", &read_fds, NULL, NULL, NULL);
   
    // 调用select接口
    int ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
    // 打印listen_fd事件是否就绪
    printf("after select:%d\n", FD_ISSET(listen_fd, &read_fds));
    // 小于0 有错误事件发生
    if(ret < 0){
      perror("select");
      continue;
    }
    // 等于0 等待超时
    if(ret == 0){
      printf("select timeout\n");
      continue;
    }

    // 处理监听事件
    if(FD_ISSET(listen_fd, &read_fds)){
      struct sockaddr_in client_addr;
      socklen_t len = sizeof(client_addr);
      // accept成功获取连接后，返回新的sock
      int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
      if(connect_fd < 0){
        perror("accept");
        continue;
      }
      printf("#client %s: %d connect\n", inet_ntoa(client_addr.sin_addr),\
          ntohs(client_addr.sin_port));
      // 将与用户交互的套接字加入到集合中
      Add(connect_fd, fd_list, sizeof(fd_list)/sizeof(int));
    }

    // 处理 connect_fd
    size_t i;
    for(i= 0; i < sizeof(fd_list)/sizeof(int); ++i){
      if(fd_list[i] == -1){
      continue;
      }
      // 遍历文件描述集中就绪事件
      if(!FD_ISSET(fd_list[i], &read_fds)){
        continue;
      }

      char buf[1024] = {0};
      ssize_t read_size = read(fd_list[i], buf, sizeof(buf)-1);
      if(read_size < 0){
        perror("read");
        continue;
      }

      //read返回值为0,表示读到EOF或socket对端关闭 
      if(read_size == 0){
        printf("Client say: goodbye\n");
        close(fd_list[i]);
        fd_list[i] = -1;
      }
      
      printf("#Client say: %s ", buf);
      
      char respond[1024] = {0};
      printf("#Serve say： ");
      scanf("%s", respond);
      write(fd_list[i], respond, strlen(respond));
    }
  }
  return 0;
}
