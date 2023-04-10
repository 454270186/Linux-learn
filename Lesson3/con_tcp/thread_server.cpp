#include <iostream>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 设置端口复用
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 绑定ip和端口
    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    soaddr.sin_port = htons(9999);
    soaddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    listen(lfd, 128);

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
        if (cfd == -1) {
            perror("accept");
            exit(-1);
        }

        // 这里如果使用引用捕获可能会出现数据竞争的问题
        std::thread th([cliaddr, cfd]() {
            // get client info
            char client_ip[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            unsigned int client_port = ntohs(cliaddr.sin_port);
            std::cout << "Tread: " << std::this_thread::get_id() << std::endl;
            printf("client ip: %s, port: %d\n", client_ip, client_port);

            while (1) {
                // 6. read client
                char recv_buf[1024] = {0};
                int len = read(cfd, recv_buf, sizeof(recv_buf));
                if (len == -1) {
                    perror("read:");
                    return -1;
                } else if (len > 0) {
                    printf("receive client data %d: %s\n", client_port, recv_buf);
                } else if (len == 0) {
                    // client closed
                    printf("client %d close...\n", client_port);
                    break;
                }

                // 7. send to client
                write(cfd, recv_buf, strlen(recv_buf));
            }

            close(cfd);
            return 0;
        });

        th.detach();
    }
}