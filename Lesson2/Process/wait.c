/*
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t wait(int* wstatus);
        功能：等待任意一个子进程结束，如果一个子进程结束了，此函数会回收子进程的资源
        参数：int* wstatus
            进程退出时的状态信息，传入的是一个int类型的地址，是一个传出参数
        返回值：
            - 成功：返回被回收的子进程的pid
            - 失败：-1

    调用wait()的进程会被阻塞，直到它的一个子进程退出或者收到一个不能被忽略的信号时才会被唤醒
    如果没有子进程了，函数立刻返回-1；如果子进程都结束了，也立刻返回-1
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // 创建5个子进程
    pid_t pid;
    for (int i = 0; i < 5; i++) {
        pid = fork();
        // 防止子进程再创建进程
        if (pid == 0) {
            break;
        }
    }

    if (pid > 0) {
        // 父进程
        while (1) {
            printf("parent, pid: %d\n", getpid());

            int st;
            int ret = wait(&st);

            // 子进程全部结束，wait返回-1
            if (ret == -1) {
                break;
            }

            if (WIFEXITED(st)) {
                // 正常退出
                printf("exit status code: %d\n", st);
            }
            if (WIFSIGNALED(st)) {
                // 是不是异常终止
                printf("killed by signal: %d\n", st);
            }

            printf("child die: %d\n", ret);
            sleep(1);
        }
    } else if (pid == 0) {
        //while (1) {
            printf("child, pid: %d\n", getpid());
            sleep(1);
        //}
    }
}