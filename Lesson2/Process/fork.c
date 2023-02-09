// create child process
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

/*
    pid_t fork()

    DESCRIPTION
        creates  a new process by duplicating the calling process.  The new process is referred to as the child process.
        The calling process is referred to as the parent process.
    

    RETURN VALUE
       On  success,  the PID of the child process is returned in the parent, and 0 is returned in the child.  On failure, -1 is
       returned in the parent, no child process is created, and errno is set appropriately.

       fork()的返回值会返回两次，一次在父进程中，一次在子进程中
       在父进程中返回创建的子进程的pid
       在子进程中返回0
       通过fork的返回值区分父进程和子进程
       在父进程中创建失败时返回-1，并设置errno
*/

int main() {
    // create child process
    pid_t pid = fork();
    printf("process created, pid: %d\n", pid);
    if (pid > 0) {
        printf("I am parent process, pid: %d, ppid: %d\n", getpid(), getppid());

    } else if (pid == 0) {
        printf("I am child process, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    // different process's for loop
    for (int i = 0; i < 3; i++) {
        printf("%d from process pid: %d\n", i, getpid());
        sleep(1);
    }
    return 0;
}

/*
    for loop output:
        process created, pid: 16031
        I am parent process, pid: 16030, ppid: 3384
        0 from process pid: 16030
        process created, pid: 0
        I am child process, pid: 16031, ppid: 16030
        0 from process pid: 16031
        1 from process pid: 16031
        1 from process pid: 16030
        2 from process pid: 16030
        2 from process pid: 16031

    说明在cpu中每个进程交替运行
*/