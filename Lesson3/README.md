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

### TCP状态转换

