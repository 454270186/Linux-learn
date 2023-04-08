#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // 1. create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket:");
        exit(-1);
    }

    // 2. connect
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("connect:");
        exit(-1);
    }

    char data[1024];
    while (scanf("%s", data)) {
        // 3. send to server
        write(fd, data, strlen(data));
        // 4. read server
        char recv_buf[1024] = {0};
        int len = read(fd, recv_buf, sizeof(recv_buf));
        if (len == -1) {
            perror("read:");
            exit(-1);
        } else if (len > 0) {
            printf("receive server data: %s\n", recv_buf);
        } else if (len == 0) {
            // client closed
            printf("server close...\n");
            break;
        }
    }

    close(fd);

    return 0;
}