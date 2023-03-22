#include <pthread.h>
#include <stdio.h>

pthread_rwlock_t rwlock;
int shared_data = 0;

void* read_data(void* arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("Read data: %d\n", shared_data);
        pthread_rwlock_unlock(&rwlock);
    }
    
    return NULL;
}

void* write_data(void* arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        shared_data++;
        printf("Write data: %d\n", shared_data);
        pthread_rwlock_unlock(&rwlock); 
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    pthread_rwlock_init(&rwlock, NULL);
    pthread_create(&t1, NULL, write_data, NULL);
    pthread_create(&t2, NULL, read_data, NULL);
    pthread_create(&t3, NULL, read_data, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}