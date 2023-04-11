#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>

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

    int optval = 1;
    int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if (ret == -1) {
        perror("setsockopt");
        exit(-1);
    }

    ret = bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    listen(lfd, 128);

    // poll
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int nfds = 0;
    while (1) {
        
        int ret = poll(fds, nfds + 1, -1);
        if (ret == -1) {
            perror("select");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 检测到有文件描述符的缓冲区发生了变化
            if (fds[0].revents & POLLIN) {
                // lfd变化说明是有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                socklen_t clilen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
                if (cfd == -1) {
                    perror("accept");
                    exit(-1);
                }
                int max_index = 0;
                for (int i = 1; i < 1024; i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        max_index = max_index > i ? max_index : i;
                        break;
                    }
                }

                nfds = nfds > max_index ? nfds : max_index;
            }

            // 遍历，读取数据
            for (int i = 1; i <= nfds; i++) {
                if (fds[i].revents & POLLIN) {
                    char recv_buf[1024] = {0};
                    int len = read(fds[i].fd, recv_buf, sizeof(recv_buf)); // 目前recv_buf并没有数据，使用strlen会导致错误
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client close...\n");
                        close(i);
                        fds[i].fd = -1; // 断开连接，将fd置空
                    } else if (len > 0) {
                        printf("receive data: %s\n", recv_buf);
                        write(fds[i].fd, recv_buf, strlen(recv_buf)+1);
                    }
                }
            }
        }
    }

    close(lfd);

    return 0;
}