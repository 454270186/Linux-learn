/*
    多线程实现卖票案例
    有3个窗口，100张票

    用mux来实现线程同步，保证数据安全
    类型： pthread_mux_t
    
    pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)：
        初始化一个mutex对象。mutex参数是指向要初始化的mutex对象的指针，attr参数是一个可选参数，可以用于设置mutex属性。

    pthread_mutex_lock(pthread_mutex_t *mutex)：
        尝试加锁，如果mutex已经被另一个线程锁定，则阻塞当前线程。

    pthread_mutex_trylock(pthread_mutex_t *mutex)：
        尝试加锁，如果mutex已经被另一个线程锁定，则返回错误码而不是阻塞线程。

    pthread_mutex_unlock(pthread_mutex_t *mutex)：释放mutex对象上的锁。
    pthread_mutex_destroy(pthread_mutex_t *mutex)：销毁一个mutex对象。
*/
#include <stdio.h>
#include <pthread.h>
int ticket = 100;
pthread_mutex_t mux;

void* sellticket(void* arg) {
    while (1) {
        pthread_mutex_lock(&mux);

        if (ticket > 0) {
            printf("tid %ld is selling the %d ticket\n", pthread_self(), ticket);
            ticket--;
        } else {
            pthread_mutex_unlock(&mux);
            break;
        }

        pthread_mutex_unlock(&mux);
    }
}

int main() {
    pthread_mutex_init(&mux, NULL);
    
    // 创建3个子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_exit(0);

    return 0;
}