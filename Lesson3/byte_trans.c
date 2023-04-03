/*
    #include <arpa/inet.h>

    // 转换端口
    uint16_t htons(uint16_t hostshort);
    uint16_t ntohs(uint16_t netshort);

    // 转IP
    uint32_t htonl(uint32_t hostlong);
    uint32_t ntohl(uint32_t netlong);
*/

#include <arpa/inet.h>
#include <stdio.h>

int main() {
    // htons
    unsigned short a = 0x0102;
    printf("a: %x\n", a);
    unsigned short b = htons(a);
    printf("b: %x\n", b);

    printf("=================\n");

    // htonl
    char buf[4] = {192, 168, 1, 100};
    int num = *(int*)buf;
    int res = htonl(num);
    unsigned char* p = (char*)&res;

    printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));

    printf("=================\n");



}