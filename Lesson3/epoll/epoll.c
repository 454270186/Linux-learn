#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &soaddr.sin_addr.s_addr);
    soaddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    listen(lfd, 8);

    int epfd = epoll_create(100);

    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024]; // 接收发生改变的文件描述符集合

    while (1) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret == %d\n", ret);

        for (int i = 0; i < ret; i++) {
            int cur_fd = epevs[i].data.fd;
            if (cur_fd == lfd) {
                // 监听到有客户端连接
                struct sockaddr_in cliaddr;
                int clilen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);

                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                // 说明有数据到达
                char recv_buf[1024] = {0};
                int len = read(cur_fd, recv_buf, sizeof(recv_buf));
                if (len == -1) {
                    perror("read");
                    exit(-1);
                } else if (len == 0) {
                    printf("client close...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cur_fd, NULL);
                    close(cur_fd);
                } else if (len > 0) {
                    printf("receive data: %s\n", recv_buf);
                    write(cur_fd, recv_buf, strlen(recv_buf)+1);
                }
            }
        }
    }

    close(lfd);
    close(epfd);

    return 0;
}