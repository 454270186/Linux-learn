#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.0", &soaddr.sin_addr.s_addr);
    soaddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    listen(lfd, 128);

    // select
    fd_set read_set; // 只允许通过FD_接口修改
    FD_ZERO(&read_set);
    FD_SET(lfd, &read_set);
    int max_fd = lfd;

    while (1) {
        fd_set temp = read_set; // temp set交给内核去检测和修改
        
        int ret = select(max_fd+1, &temp, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 检测到有文件描述符的缓冲区发生了变化
            if (FD_ISSET(lfd, &temp)) {
                // lfd变化说明是有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                socklen_t clilen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
                if (cfd == -1) {
                    perror("accept");
                    exit(-1);
                }

                FD_SET(cfd, &read_set);

                max_fd = max_fd > cfd ? max_fd : cfd;
            }

            // 遍历，读取数据
            for (int i = lfd + 1; i <= max_fd; i++) {
                if (FD_ISSET(i, &temp)) {
                    char recv_buf[1024] = {0};
                    int len = read(i, recv_buf, sizeof(recv_buf)); // 目前recv_buf并没有数据，使用strlen会导致错误
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client close...\n");
                        close(i);
                        FD_CLR(i, &read_set);
                    } else if (len > 0) {
                        printf("receive data: %s\n", recv_buf);
                        write(i, recv_buf, strlen(recv_buf)+1);
                    }
                }
            }
        }
    }

    close(lfd);

    return 0;
}