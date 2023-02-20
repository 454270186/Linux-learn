#include <unistd.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int ret = access("test", F_OK);
    if (ret == -1) {
        printf("管道不存在 创建管道\n");

        ret = mkfifo("test", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    int fd = open("test", O_WRONLY); // 如果没有读端，会阻塞在这
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    for (int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello %d", i);
        printf("write: %s\n", buf);

        write(fd, buf, strlen(buf));
        
    }

    close(fd);

    return 0;
}