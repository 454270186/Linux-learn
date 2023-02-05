#include <stdio.h>
#include "calc.h"

int main() {
    int a = 2;
    int b = 4;

    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", sub(a, b));
    printf("a * b = %d\n", mul(a, b));
    printf("a / b = %d\n", div(a, b));

    return 0;
}
