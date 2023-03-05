#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int src_fd = open("src.txt", O_RDWR);
    if (src_fd == -1) {
        perror("src_open");
        exit(0);
    }
    int size = lseek(src_fd, 0, SEEK_END);

    int dest_fd = open("dest.txt", O_RDWR | O_CREAT);
    truncate("dest.txt", size);

    void* src_ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, src_fd, 0);
    if (src_ptr == MAP_FAILED) {
        perror("src_mmap");
        exit(0);
    }

    void* dest_ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, dest_fd, 0);
    if (dest_ptr == MAP_FAILED) {
        perror("dest_mmap");
        exit(0);
    }

    memcpy(dest_ptr, src_ptr, size);

    // release resouces
    // 为了安全，释放资源的顺序最好按照栈的顺序(先创建的后释放)
    munmap(dest_ptr, size);
    munmap(src_ptr, size);

    close(dest_fd);
    close(src_fd);

    return 0;

}