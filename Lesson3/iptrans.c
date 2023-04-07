#include <stdio.h>
#include <arpa/inet.h>

int main() {
    // creare an IP string, dots-and-numbers
    char buf[] = "192.168.1.4";
    unsigned int num = 0;

    inet_pton(AF_INET, buf, &num); // convert dot-number IP to net int
    unsigned char* p = (unsigned char*)&num;
    printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));

    // convert net int to dot-number ip
    char ip[16];
    const char* str = inet_ntop(AF_INET, &num, ip, sizeof(ip));

    printf("%s\n", str);
}