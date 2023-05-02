#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    fork();
    printf("A\n");
    fork();
    printf("B\n");
    return 0;
}
