#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    struct sigaction new_action;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_handler = SIG_IGN;
    new_action.sa_flags = 0;

    sigaction(SIGINT, &new_action, NULL);

    printf("Useless spin\n");
    printf("My PID = %d\n", getpid());
    while(1);
}
