/* fgets example */
#include <stdio.h>

int main() {
    char str[100];

    fgets(str, 99, stdin);

    puts(str);
}