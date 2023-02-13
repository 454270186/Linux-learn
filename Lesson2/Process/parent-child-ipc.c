/*
    实现 ps aux | grep xxx 父子进程间通信

    子进程：ps aux， 子进程结束后，将数据发送给父进程
    父进程：接受数据，打印

    pipe()
    execlp()
    子进程将标准输出 stdout_fileno 重定向到管道的写端 dup2()
*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // parent process
        // 父进程只读，关闭写端
        close(pipefd[1]);

        int len = -1;
        char buf[1024];
        while ((len = read(pipefd[0], buf, sizeof(buf) - 1) > 0)) {
            printf("%s", buf);
        }

    } else if (pid == 0) {
        // child process
        // 子进程只写，为了程序健壮性，将子进程管道读端关闭
        close(pipefd[0]);

        // 重定向ps aux的输出到管道写端
        dup2(pipefd[0], STDOUT_FILENO);

        execlp("ps", "ps", "aux", NULL);
        perror("ps aux");
        exit(0);
    }
}