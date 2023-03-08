/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);
    DESCRIPTION
       The  pthread_detach() function marks the thread identified by thread as detached.  When a detached thread terminates, its resources are
       automatically released back to the system without the need for another thread to join with the terminated thread.

       Attempting to detach an already detached thread results in unspecified behavior.
    
    - 注意：
        - 不能多次分离
        - 不能join一个已经分离的线程

*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* callback(void* arg) {

}

int main() {
    pthread_t tid;

    pthread_create(&tid, NULL, callback, NULL);
}