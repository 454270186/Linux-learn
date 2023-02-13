/*
    #include <unistd.h>
    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用来进程间通信
        参数：int pipefd[2] 是一个传出参数
            pipefd[0] 对应管道的写端
            pipefd[1] 对应管道的读端
        返回值：
            成功 0
            失败 -1

    管道默认时阻塞的：如果管道中没有数据，read()阻塞；如果管道满了，write()阻塞

    匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 子进程发送数据，父进程接受
int main() {
    // 管道要在fork前创建，否则就不是相同的管道了
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid > 0) {
        // parent process
        printf("I am parent process, pid = %d\n", getpid());
        char buf[1024];

        while (1) {
            read(pipefd[0], buf, sizeof(buf));
            printf("parent receive msg: %s\n", buf);

            char* str = "hello, I am parent";
            write(pipefd[1], str, strlen(str));

            sleep(1);
        }
        

    } else if (pid == 0) {
        // child process
        //sleep(10); // if child sleep for 10s, read() in parent will block parent until child write sth in channel
        printf("I am child process, pid = %d\n", getpid());
        char buf[1024];
        while (1) {
            char* str = "hello, I am child";
            write(pipefd[1], str, strlen(str));

            sleep(1);

            read(pipefd[0], buf, sizeof(buf));
            printf("child reveive msg: %s\n", buf);
        }

    }

    return 0;
}