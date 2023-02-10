#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // create child process
    pid_t pid = fork();
    printf("process created, pid: %d\n", pid);
    if (pid > 0) {
        while (1) {
           printf("I am parent process, pid: %d, ppid: %d\n", getpid(), getppid()); 
           sleep(1);
        }

    } else if (pid == 0) {
        printf("I am child process, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    // different process's for loop
    for (int i = 0; i < 3; i++) {
        printf("%d from process pid: %d\n", i, getpid());
    }
    return 0;
}

/*
    - ps aux 显示：
    xiaofei     6146  0.0  0.0   2772   944 pts/5    S+   22:55   0:00 ./zombie
    xiaofei     6147  0.0  0.0      0     0 pts/5    Z+   22:55   0:00 [zombie] <defunct>

    子进程变为僵尸进程
*/