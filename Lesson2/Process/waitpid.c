/*
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t waitpid(pid_t pid, int* wstatus, int options);
        -功能：回收指定进程号的子进程，可以设置是否阻塞
        -参数：
            -pid:
                pid > 0: 指定的子进程pid
                pid = 0: 回收当前进程组的所有子进程
                pid = -1: 回收所有子进程，相当于wait()
                pid < -1: 回收组id为pid的绝对值的进程组中的子进程
            -wstatus：
                与wait()的参数相同
            -options；设置阻塞或非阻塞
                0：阻塞
                WNOHANG：非阻塞
        
        -返回值：
            > 0 : 返回子进程的pid
            = 0 : 当options设置为WNOHANG时返回0，表示还有子进程或者
            = -1 : 错误 或者 没有子进程了


*/

// 演示不阻塞的情况
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
            sleep(1);

            int st;
            int ret = waitpid(-1, &st, WNOHANG); // 回收全部子进程并且不阻塞

            if (ret == -1) {
                // 子进程全部结束，waitpid返回-1
                break;
            } else if (ret == 0) {
                // 说明还有子进程活着，跳过
                continue;
            } else if (ret > 0) {
                // 回收了特定的一个子进程，判断子进程信息
                if (WIFEXITED(st)) {
                    // 正常退出
                    printf("exit status code: %d\n", st);
                }
                if (WIFSIGNALED(st)) {
                    // 是不是异常终止
                    printf("killed by signal: %d\n", st);
                }

                printf("child die: %d\n", ret);
            }
        }
    } else if (pid == 0) {
        while (1) {
            printf("child, pid: %d\n", getpid());
            sleep(1);
        }
    }
}