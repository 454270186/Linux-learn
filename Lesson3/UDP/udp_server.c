#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    soaddr.sin_port = htons(9999);
    soaddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&soaddr, sizeof(soaddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }
    
    int num = 0;
    while (1) {
        char recvbuf[128];
        char sendbuf[128];
        char cli_ip[16];

        struct sockaddr_in cliaddr;
        int clilen = sizeof(cliaddr);

        // receive data
        int ret = recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&cliaddr, &clilen);
        if (ret == -1) {
            perror("recvfrom");
            exit(-1);
        }

        printf("client ip: %s, port: %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cli_ip, 16), ntohs(cliaddr.sin_port));
        printf("receive data: %s\n", recvbuf);

        // send data
        sprintf(sendbuf, "server: fuck you %d times\n", num++);
        sendto(fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    }

    close(fd);
    return 0;
}