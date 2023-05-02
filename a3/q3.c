#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int x= fork();
    printf("%d\n", x);
    return 0;
}
