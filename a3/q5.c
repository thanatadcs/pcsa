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
    wait(&ret);
    printf("r: %d\n", ret);
    printf("s: %d\n", WIFEXITED(ret));
    printf("s: %d\n", WEXITSTATUS(ret));
    printf("r: %d\n", WIFSIGNALED(ret));
    printf("r: %d\n", WTERMSIG(ret));
    printf("r: %d\n", WCOREDUMP(ret));
    printf("r: %d\n", WIFSTOPPED(ret));
    printf("r: %d\n", WSTOPSIG(ret));
    printf("r: %d\n", WIFCONTINUED(ret));
    printf("Done\n");
    return 0;
}
