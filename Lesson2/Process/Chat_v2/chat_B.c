#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 128

/*
    parent A -> chat1 -> child B
    child A  <- chat2 <- parent B
*/

int main() {
    int ret = access("chat1", F_OK);
    if (ret == -1) {
        printf("FIFO1 does not exit, create FIFO1\n");

        ret = mkfifo("chat1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("chat2", F_OK);
    if (ret == -1) {
        printf("FIFO2 does not exit, create FIFO2\n");

        ret = mkfifo("chat2", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    pid_t pid = fork();

    if (pid > 0) {
        // parent
        int w_fd = open("chat2", O_WRONLY);
        char buf[BUF_SIZE];

        while (1) {
            memset(buf, 0, BUF_SIZE);
            fgets(buf, BUF_SIZE, stdin);
            write(w_fd, buf, strlen(buf));
        }

    } else if (pid == 0) {
        // child
        int r_fd = open("chat1", O_RDONLY);
        char buf[BUF_SIZE];

        while (1) {
            memset(buf, 0, BUF_SIZE);
            int len = read(r_fd, buf, sizeof(buf));

            printf("received from chatA: %s", buf);
        }
    } else {
        perror("fork");
        exit(0);
    }

}