#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 128

/*
    1. create two fifo channels:
        chatA -> chat1 -> chatB
        chatA <- chat2 <- chatB

    2. chatA: write -> read
    3. chatB: read -> write
*/

int main() {
    int ret = access("chat1", F_OK);
    if (ret == -1) {
        printf("FIFO_1 does not exist, create one\n");

        ret = mkfifo("chat1", 0664);
        if (ret == -1) {
            perror("mkfifo1");
            exit(0);
        }

    }

    ret = access("chat2", F_OK);
    if (ret == -1) {
        printf("FIFO_2 does not exist, create one\n");

        ret = mkfifo("chat2", 0664);
        if (ret == -1) {
            perror("mkfifo2");
            exit(0);
        }
    }

    int read_fd = open("chat1", O_RDONLY);
    int write_fd = open("chat2", O_WRONLY);

    char buf[BUF_SIZE];
    while (1) {
        memset(buf, 0, BUF_SIZE);
        int len = read(read_fd, buf, sizeof(buf));
        if (len <= 0) {
            perror("read");
            break;
        }
        printf("receive from chatA: %s\n", buf);

        memset(buf, 0, BUF_SIZE);
        fgets(buf, BUF_SIZE, stdin);
        write(write_fd, buf, strlen(buf));
    }

    close(read_fd);
    close(write_fd);
}