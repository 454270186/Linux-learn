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

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char sendbuf[128];
        char recvbuf[128];

        // send data
        sprintf(sendbuf, "client sends data %d\n", num++);
        sendto(fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        recvfrom(fd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        printf("receive data: %s\n", recvbuf);

        sleep(2);
    }

    close(fd);
    return 0;
}