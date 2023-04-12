#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

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

                // 将文件描述符设置为非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                epev.events = EPOLLIN | EPOLLET; // 设置边沿触发
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                // 说明有数据到达
                // 循环读取出所有数据
                char buf[5] = {0};
                int len = 0;
                while ((len = read(cur_fd, buf, sizeof(buf))) > 0) {
                    printf("receive data: %s\n", buf);
                    write(cur_fd, buf, strlen(buf));
                }

                if (len == 0) {
                    printf("client close...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cur_fd, NULL);
                    close(cur_fd);
                } else if (len == -1) {
                    if (errno == EAGAIN) {
                        printf("read finished...\n");
                        continue;
                    }
                    perror("read");
                    exit(-1);
                }
            }
        }
    }

    close(lfd);
    close(epfd);

    return 0;
}