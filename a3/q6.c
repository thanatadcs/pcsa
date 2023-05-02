#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    for (int i=0;i<5;i++) {
        if (fork() == 0) {
            sleep(1);
            printf("%d\n", i);
            return 0;
        }
    }
    int ret;
    for (int i=0;i<5;i++)
        wait(&ret);
    printf("Done\n");
    return 0;
}
