# Process

## 进程的状态
**三态模型**
- 就绪态
  进程具备运行条件，等待系统分配处理器运行

- 运行态
  进程正在占用处理器运行

- 阻塞态
  进程不具备运行条件，正在等待某个事件的完成

在一个系统中处于就绪状态的进程可能有多个，被排成就绪队列

**五态模型**
- 新建态
  进程刚被创建，还未进入就绪队列
- 就绪态
- 运行态
- 阻塞态
- 终止态


## 父子进程
**执行**
使用fork()创建子进程
父进程和子进程都会执行main里的代码
在父进程中fork()返回父进程pid
在子进程中fork()返回0
```c
int main() {
    // create child process
    pid_t pid = fork();
    printf("process created, pid: %d\n", pid);
    if (pid > 0) {
        printf("I am parent process, pid: %d, ppid: %d\n", getpid(), getppid());

    } else if (pid == 0) {
        printf("I am child process, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    // different process's for loop
    for (int i = 0; i < 3; i++) {
        printf("%d from process pid: %d\n", i, getpid());
        sleep(1);
    }
    return 0;
}
```

**父子进程虚拟地址空间**

调用fork()后会克隆父进程的虚拟地址空间到子进程
fork()调用后，子进程的用户区的数据和父进程的一样。内核区也会拷贝（除了pid）

**fork()原理--copy on write**

Linux的fork()使用是通过写时拷贝实现
写时拷贝是一种可以推迟甚至避免拷贝的技术。
内核此时并不复制整个进程的地址空间，而是让父子进程共享一个内存空间。只有在需要写入的时候才会复制地址空间，从而使每个进程拥有各自的内存地址空间。
在写入之前，内存地址以只读方式共享。


## 进程退出
look [exit() and _exit()](./exit.ck)
```C
// 子进程退出的状态码会被父进程获取

// 标准C库
#include <stdio.h>
void exit(int status);

// Linux系统函数
#include <unistd.h>
void _exit(int status);

```

## 孤儿进程
父进程的义务是回收当子进程结束时回收子进程pcb的资源

- 父进程结束，但子进程还在运行（尚未结束），这样的子进程被称为孤儿进程

- 每当一个孤儿进程出现时，内核就把孤儿进程的父进程设置为init(pid为1)， init进程会循环地wait()这些孤儿进程，当一个孤儿进程结束生命周期的时候，init进程负责善后

- 所以孤儿进程不会有什么危害


## 僵尸进程

- 每个进程结束后，都会释放自己地址空间中的用户区数据，内核区的pcb没有办法自己释放掉，需要父进程区释放

- 进程终止时，父进程尚未回收，子进程残留资源(PCB)存放于内核中，编程僵尸进程

- 僵尸进程不能被 kill -9 杀死

- 如何父进程不调用wait() or waitpid()的话，那么保留的那段信息就不会被释放，其进程号就会一直被占用。所以如果大量的产生僵尸进程，将会因为没有可用的进程号而导致不能产生新的进程，危害很大，应当避免


## 进程回收
使用wait() and waitpid()
