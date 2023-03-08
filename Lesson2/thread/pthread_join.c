/*
    #include <pthread.h>
    int pthread_join(pthread_t thread, void **retval);
        - 功能：和一个已经终止的线程进行连接
                回收子线程的资源
                阻塞函数，调用一次只能回收一个子线程
                一般在主线程中使用
        - 参数：
            - thread: 需要回收的子线程的ID
            - retval: 接受子线程退出时的返回值 (注意是一个二级指针)
        - 返回值：
            0 ：成功
            非0 ：失败，返回错误号

*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int global_value = 10;

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());

    //return NULL; // phread_exit(NULL);

    // value是一个局部变量，无法返回给主线程
    // int value = 10;
    // pthread_exit((void*)&value);

    pthread_exit((void*)&global_value);
}


int main() {
    pthread_t tid; 

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        printf("error: %s\n", strerror(ret));
    }

    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    // 主线程调用pthread_join()回收子进程的资源
    int* thread_rtval;
    ret = pthread_join(tid, (void**)&thread_rtval);
    if (ret != 0) {
        printf("error: %s\n", strerror(ret));
    }
    printf("child thread returns data: %d\n", *thread_rtval);

    pthread_exit(NULL);
    return 0;
}