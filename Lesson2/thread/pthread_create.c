/*
    #include <pthread.h>

    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        void *(*start_routine) (void *), void *arg);

    - 参数：
        - thread: 传出参数，线程创建成功后，子线程的id被写入此变量
        - attr: 设置线程的属性，一般为NULL
        - start_routine: 函数指针，这个函数是子线程需要处理的逻辑代码
        - arg: 给第三个参数使用，传参

    - 返回值:
        成功：0
        失败：返回错误号，这个错误号和errno不太一样
        获取错误号的信息：char* strerror(int errnum);



    进程退出
    void pthread_exit(void *retval);
    - 参数：
        -The  pthread_exit() function terminates the calling thread and returns a value via retval

    pthread_t pthread_self(void);
    - 功能：获取当前线程的线程ID
    
    int pthread_equal(pthread_t t1, pthread_t t2);
    - 功能：比较两个线程ID是否相等
    因为不同操作系统，phthread_t类型的实现不一样，不能直接比较
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void* callback(void* arg) {
    printf("num from main(): %d\n", *(int*)arg);
    printf("da sha bi zai ci\n");

    return NULL; // phread_exit(NULL);
}


int main() {
    pthread_t tid; 

    int num = 10;
    int ret = pthread_create(&tid, NULL, callback, (void*)&num); // 通过第四个参数传递参数
    if (ret != 0) {
        printf("error: %s\n", strerror(ret));
    }

    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }


    return 0;
}