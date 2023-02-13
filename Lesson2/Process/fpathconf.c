#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

// 使用 fpathconf 获取管道大小
int main() {
    int pipefd[2];
    pipe(pipefd);

    long size = fpathconf(pipefd[0], _PC_PIPE_BUF); // 第二个参数指定要获取管道缓冲区大小

    printf("pipe buf size: %ld\n", size);

    return 0;
}