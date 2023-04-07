#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // 1. create socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket: ");
        exit(-1);
    }

    // 2. bind ip and port
    // Addr_Family + ip + port
    struct sockaddr_in soaddr;
    soaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &soaddr.sin_addr.s_addr);
    soaddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr*)&soaddr, sizeof(soaddr));

    // 3. listen
    listen(lfd, 8);

    // 4. accept
    struct sockaddr_in client_addr; // socket address of client
    socklen_t client_len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr*)&client_addr, &client_len);
    if (cfd == -1) {
        perror("accept:");
        exit(-1);
    }

    // 5. output client info
    char client_ip[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    unsigned int client_port = ntohs(client_addr.sin_port);
    printf("client: ip is %s, port is %d\n", client_ip, client_port);

    while (1) {
        // 6. read client
        char recv_buf[1024] = {0};
        int len = read(cfd, recv_buf, sizeof(recv_buf));
        if (len == -1) {
            perror("read:");
            exit(-1);
        } else if (len > 0) {
            printf("receive client data: %s\n", recv_buf);
        } else if (len == 0) {
            // client closed
            printf("client close...\n");
            break;
        }

        // 7. send to client
        write(cfd, recv_buf, strlen(recv_buf));
    }


    // 8. close
    close(lfd);
    close(cfd);

    return 0;
}