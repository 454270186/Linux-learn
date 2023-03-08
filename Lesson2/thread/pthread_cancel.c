/*
    #include <pthread.h>
    int pthread_cancel(pthread_t thread);
        - 功能：取消线程（让线程终止）
            取消某个线程可以终止某个线程的运行，
            但不是马上终止，而是当此线程执行到一个取消点，线程才会终止。
            取消点：一般是一些系统调用函数
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callback(void* arg) {
    for (int i = 0; i < 10; i++) {
        printf("child: %d\n", i);
        sleep(1);
    }
}

int main() {
    pthread_t tid;

    pthread_create(&tid, NULL, callback, NULL);

    pthread_cancel(tid); // 向子线程发送取消请求

    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
        sleep(1);
    }

    return 0;
}