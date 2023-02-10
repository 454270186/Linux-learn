#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // create child process
    pid_t pid = fork();
    printf("process created, pid: %d\n", pid);
    if (pid > 0) {
        printf("I am parent process, pid: %d, ppid: %d\n", getpid(), getppid());

    } else if (pid == 0) {
        sleep(1);
        printf("I am child process, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    // different process's for loop
    for (int i = 0; i < 3; i++) {
        printf("%d from process pid: %d\n", i, getpid());
    }
    return 0;
}

/*
    子进程sleep后，父进程结束，子进程变为孤儿进程，再打印for循环，此时ppid变为1
*/