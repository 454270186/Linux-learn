#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*
    // 标准C库
    #include <stdio.h>
    void exit(int status);

    // Linux系统函数
    #include <unistd.h>
    void _exit(int status);

    -区别
        调用exit()后会：
        1. 调用退出处理函数
        2. 刷新I/O缓冲 关闭文件描述符
        3. 最后调用_exit()系统调用 -> 结束进程

        调用_exit()则会直接结束进程
*/

int main() {
    printf("hello\n"); // 换行符刷新缓冲区
    printf("world");

    exit(0);
    //_exit(0);
}

/*
    因为_exit()不会刷新缓冲区，所以调用_exit()结束进程时world没有被打印出来
*/