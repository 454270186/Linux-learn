#include <unistd.h>
#include <stdio.h>
/*
    #include <unistd.h>
    int execlp(const char* file, const char* arg, ...)
        会到环境变量中查找指定的可执行文件，如果找到了就执行
        -参数：
            - file: 需要指定的执行的文件的文件名

            - arg: 执行可执行文件所需要的参数列表
                   第一个arg一般是执行的程序的名称
                   从第二个参数开始，就是程序执行所需要的参数列表
                   参数最后需要以NULL结尾

        - 返回值：
            只有当调用失败才会返回-1
            调用成功时没有返回值
*/

int main() {
    // 创建子进程，调用hello.out
    pid_t pid = fork();

    if (pid > 0) {
        printf("I am parent process, pid: %d\n", getpid());
        sleep(1);
    }
    else if (pid == 0) {
        // 在子进程中调用ps
        execlp("ps", "ps", "aux", NULL);
    }

    for (int i = 0; i < 3; i++) {
        printf("%d for loop from %d\n", i, getpid());
    }

    return 0;
}