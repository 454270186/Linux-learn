#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int shmid = shmget(1, 4096, IPC_CREAT | 0664);

    void* ptr = shmat(shmid, NULL, 0);

    char* str = "hello world";

    printf("%s\n", (char*)ptr);
    printf("wait..\n");
    getchar();

    shmdt(ptr);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}