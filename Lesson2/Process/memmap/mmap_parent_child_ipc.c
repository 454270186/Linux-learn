/*
    #include <sys/mman.h>

    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
        -功能：将一个文件或设备的数据映射到内存中
        -参数：
            - void* addr: The starting address for the new mapping is speci‐fied in addr.
                          一般设置为NULL，由内核指定位置
            - length: 要映射的数据的长度，需要>=0，建议使用文件的长度
            - prot: 对申请的内存映射区的操作权限
                - PROT_EXEC  Pages may be executed.

                - PROT_READ  Pages may be read.

                - PROT_WRITE Pages may be written.

                - PROT_NONE  Pages may not be accessed.
                要操作映射区，必须得有读权限
                读写权限：PROT_READ|PROT_WRITE

            - flags: 
                -MAP_SHARED: 映射区的数据会自动与磁盘文件进行同步（进程间通信必须设置）
                -MAP_PRIVATE: 不同步，内存映射区的数据改变了对原来的文件不会修改，会创建一个新文件(copy on write)

            - fd: 需要映射的文件的文件描述符
                -通过open得到，open的是一个磁盘文件
                -注意：文件大小不能为0，open的权限不能与prot的权限有冲突
                    prot: PROT_READ ===> open: read or read/write
                    prot: PROT_READ | PROT_WRITE ===> open: read/write

        -返回值：
            -成功：returns a pointer to the mapped area
            -失败：the value MAP_FAILED (that is, (void *) -1) is returned

    int munmap(void *addr, size_t length);
        -功能：释放内存映射
        -参数：
            -addr：需要释放的首地址
            -length：需要与mmap中的length对应
*/