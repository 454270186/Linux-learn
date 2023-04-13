## TCP 通信流程
### 服务器端
1. 创建一个用于监听的套接字
    - 监听： 监听客户端的连接
    - 套接字：实际上是一个文件描述符

2. 将监听文件描述符和本地的IP和端口号绑定（IP和端口是服务器的地址信息）
3. 设置监听：监听的fd开始工作
4. 阻塞等待：当有客户端发起连接，解除阻塞，接受客户端的连接后会得到一个和客户端通信的套接字（fd）
5. 通信：
    - 接收数据
    - 发送数据
6. 通信结束断开连接


### 客户端
1. 创建一个用于通信的套接字（fd）
2. 连接服务器，需要指定连接服务器的IP和端口
3. 连接成功后，客户端可以直接和服务器通信
    - 接收数据
    - 发送数据
4. 通信结束，断开连接

### Socket
```c++
    int socket(int domain, int type, int protocol);
    - 功能：创建一个套接字
    - 参数：
        - domain: 协议族
            AF_INET: ipv4
            AF_INET6: ipv6
            AF_UNIX, AF_LOCAL: 本地套接字通信（进程间通信）
        - type：同学过程中使用的协议类型
            SOCK_STREAM: 流式协议
            SOCK_DGRAM: 报式协议
        - protocol: 具体的一个协议，一般写0
            SOCK_STREAM: 流式协议默认使用TCP
            SOCK_DGRAM: 报式协议默认使用UDP
        - 返回值：
            - 成功：返回文件描述符，操作的是内核缓冲区
            - 失败：-1

    int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
    - 功能： 绑定，将fd和本地的 IP+端口 进行绑定
    - 参数：
        - sockfd：通过socket()得到的文件描述符
        - addr：需要绑定的socket地址，这个地址封装了ip和端口号信息
        - addrlen：第二个参数addr结构体占的内存大小

    int listen(int sockfd, int backlog);
    - 功能： 监听这个socket上的连接
    - 参数：
        - sockfd：通过socket()得到的文件描述符
        - backlog：未连接队列和已连接队列的最大值

    int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
    - 功能：接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接
    - 参数：
        - sockfd：用于监听的文件描述符
        - addr：传出参数，记录了连接成功后客户端的地址信息（IP，port）
        - addrlen: 指定第二个参数的对应内存大小
    - 返回值：
        - 成功：用于通信的文件描述符
        - 失败：-1

    int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
    - 功能：客户端连接服务器
    - 参数：
        - sockfd: 用于通信的文件描述符
        - addr：客户端要连接的服务器的地址信息
        - addrlen：第二个参数的内存大小
    - 返回值：
        - 成功：0
        - 失败：-1
```

<br>
<br>

### TCP状态转换（假设是客户端主动关闭）

**服务端**
1. listen状态：服务器调用listen函数并等待客户端连接请求。
2. syn-received状态：服务器收到客户端的SYN报文并发送SYN+ACK报文作为响应
3. established状态：服务器收到客户端的ACK报文，连接建立成功
4. close_wait状态：收到客户端发来的关闭请求，发送确认，等待客户端关闭连接
5. last_ack状态：关闭连接时，等待客户端最后的确认
6. closed状态：完成连接关闭过程，等待下一次连接请求

**客户端**
1. closed状态：客户端未发起连接请求，处于关闭状态。
2. syn-sent状态：客户端向服务器发送SYN报文，表示建立连接请求。
3. established状态：服务器发送SYN+ACK报文响应，客户端发送ACK报文，连接建立成功。
4. fin_wait1状态：发送关闭请求FIN，等待服务端确认ACK
5. fin_wait2状态：服务端发送关闭请求FIN，等待服务端关闭连接
6. time_wait状态：等待2MSL（最大报文寿命）后关闭连接
7. closed状态：完成连接关闭过程，等待下一次连接请求

#### 半关闭
半关闭是指在一个TCP连接中，其中一个端口发送了FIN包（表示数据发送完成），但是仍然可以接受另一个端口发送的数据。也就是说，在半关闭状态下，一个TCP连接的一端（比如客户端）已经结束了它的数据发送任务，但仍可以接收来自另一端（比如服务端）发送的数据。这种状态可以用于实现一些需要逐步关闭连接的场景，例如在客户端发送完请求后，服务端需要一些时间来处理请求，但是客户端不希望一直等待，而是先关闭发送端口。
```c++
// API实现
#include <sys/socket.h>

int shutdown(int sockfd, int how);
// 和close()的区别在于：
// 1. close()是减少套接字的引用计数来中止连接，shutdown()不考虑引用计数，直接关闭连接
// 2. 多进程中，一个进程调用了shutdown()关闭读写后，其他进程将无法通信；但一个进程调用close()后不会影响其他的进程
```

### 端口复用

用途：
- 防止服务器重启时之前绑定的端口还未释放
- 程序突然退出而系统没有释放端口
```c++
#include <sys/types.h>
#include <sys/socket.h>
// 设置套接字的属性
// 可以设置端口复用
int setsockopt(int sockfd, int level, int optname, const void* optval, socket_len optlen);
    参数：
        - sockfd：要操作的socket fd
        - level：级别 - SOL_SOCKET (端口复用的级别)
        - optname：选项的名称
            - SO_REUSEADDR
            - SO_REUSEPORT
        - optval: 端口复用的值（整形）
            - 1：可以复用
            - 2：不可复用
        - optlen：optval参数的大小
```
设置端口复用要在服务器绑定端口之前


### I/O多路复用

#### select

主要思想：
1. 首先构建一个关于文件描述符的列表，将要监听的文件描述符添加到该列表中
2. 调用一个系统函数，监听该列表中的文件描述符，直到这些描述符中一个或多个进行I/O操作时，该函数才返回
    - 这个函数是阻塞的
    - 函数对文件描述符的检测是由内核完成的

3. 在返回时，它会告诉进程有多少描述符要进行I/O操作

```c++
// sizeof(fd_set) = 128 bytes = 1024 bits
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    - 参数：
        - nfds：委托内核检测的最大文件描述符的值 + 1
        - readfds：要检测的文件描述符的读的集合
            - 一般检测读操作
            - 对应的是对方发送过来的数据，检测的是读缓冲区
            - 是一个传入传出参数
        - writefds：要检测的文件描述符的写的集合
            - 委托内核检测写缓冲区是不是还可以写数据
        - exceptfds：检测发生异常的文件描述符的集合
        - timeout：设置的超时时间
            struct timeval {
                long tv_sec;
                long tc_usec;
            };
            - NULL：永久阻塞
            - tv_sec = 0 tv_usec = 0：不阻塞
            - tv_sec > 0 tv_usec > 0：阻塞对应的时间
    
    - 返回值：
        - -1：失败
        - >0(n)：检测到集合中有n个文件描述符发生了变化


// 将参数文件描述符fd对应的标志位设置为0
void FD_CLR(int fd, fd_set *set);
// 判断fd对应的标志位是0还是1
int FD_ISSET(int fd, fd_set *set);
// 将参数文件描述符fd对应的标志位设置为1
void FD_SET(int fd, fd_set *set);
// fd_set一共有1024 bits，全部初始化为0
void FD_ZERO(fd_set* set);
```

<br>

#### poll
```c++
#include <poll.h>
struct pollfd {
    int fd; // 委托内核检测的文件描述符
    short events; // 委托内核检测文件描述符的事件
    short revents; // 文件描述符实际发生的事件
};
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
    - 参数：
        - fds：需要检测的文件描述符的集合
        - nfds：第一个参数数组中最后一个有效元素的下标+1
        - timeout：阻塞时长
            - 0：不阻塞
            - -1：阻塞，当检测到fd变化时再解除阻塞
            - >0：阻塞的时长 millisecond
    - 返回值：
        - -1：失败
        - >0(n)：检测到集合中有n个文件描述符发生了变化
```

<br>

#### epoll
```c++
#include <sys/epoll.h>

// 创建一个新的epoll实例（在内核区），包含两个比较重要的数据：
// 1.需要检测的文件描述符的信息（红黑树储存）
// 2.就绪列表，存放检测到数据改变的文件描述符（双向链表）
int epoll_create(int size);
    - 参数：
        size：Linux 2.6.8后被忽略了，但是必须大于0
    
    - 返回值：
        -1：失败
        >0：用于操作epoll实例的文件描述符

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    - 参数：
        - epfd：epoll实例对应的文件描述符
        - op：要进行什么操作
            - EPOLL_CTL_ADD 添加
            - EPOLL_CTL_MOD 修改
            - EPOLL_CTL_DEL 删除
        - fd：要检测的文件描述符
        - event：检测文件描述符什么事件

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
    - 参数： 
        - edfd：epoll实例的fd
        - events：传出参数，保存了发生了变化的文件描述符的信息
        - maxevents: 第二个参数结构体数组的大小
        - timeout：阻塞时间
            - 0：不阻塞
            - -1：阻塞，直到检测到fd数据发生变化，解除阻塞
            - >0: 阻塞的时长 millisecond
    
    - 返回值：
        - 成功，返回发送变化的文件描述符的个数 >0
        - 失败，-1
```

**两种工作模式**

- LT模式（水平触发）

- ET模式（边沿触发） 



## UDP通信

```c++
#include <sys/types.h>
#include <sys/socket.h>

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    - 参数：
        - sockfd：通信的fd
        - buf：要发送的数据
        - len：发送的数据的长度
        - flags：0
        - dest_addr：通信的另一端的地址
        - addrlen：地址的内存大小

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
    - 参数：
        - sockfd：通信的fd
        - buf：接受的数据的数组
        - len：数组的大小
        - flags：0
        - src_addr：用来保存另一端的地址信息，不需要可以指定为NULL
        - addrlen：地址的内存大小
```