#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handler(int signum)
{
    printf("Signal %d received\n", signum);
    printf("Bye\n");
    exit(0);
}

int main()
{
    struct sigaction new_action;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_handler = handler;
    new_action.sa_flags = 0;

    sigaction(SIGINT, &new_action, NULL);

    printf("Useless spin\n");
    printf("My PID = %d\n", getpid());
    while(1);
}
