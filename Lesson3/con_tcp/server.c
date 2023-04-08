#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    soaddr.sin_port = htons(9999);
    soaddr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));

    listen(lfd, 128);

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t cli_len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &cli_len);
        if (cfd == -1) {
            perror("accept");
            exit(-1);
        }

        // 创建一个子进程来跟客户端通信
        pid_t pid = fork();
        if (pid == 0) {
            // get client info
            char client_ip[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            unsigned int client_port = ntohs(cliaddr.sin_port);
            printf("client ip: %s, port: %d\n", client_ip, client_port);

            while (1) {
                // 6. read client
                char recv_buf[1024] = {0};
                int len = read(cfd, recv_buf, sizeof(recv_buf));
                if (len == -1) {
                    perror("read:");
                    exit(-1);
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
            exit(0);
        }

    }

    close(lfd);
}