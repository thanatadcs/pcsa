#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#include "pcsa_net.h"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void copy_logic(int connFd, int outFd) {
    char buf[BUFSIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(connFd, buf, BUFSIZE)) > 0) {
        printf("DEBUG: Read %ld bytes\n", bytesRead);

        write_all(outFd, buf, bytesRead);
    }
    if (bytesRead == 0)
        printf("DEBUG: Connection closed by the client");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "bad argument: ./recv <portNum> <outputFilename>\n");
        return -1;
    }

    int listenFd = open_listenfd(argv[1]);
    if (listenFd == -1) {
        fprintf(stderr, "Failed to create socket\n");
        return -1;
    }

    int outFd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (outFd < 0) {
        fprintf(stderr, "Failed to create file\n");
        return -1;
    }

    for (;;) {
        struct sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(struct sockaddr_storage);

        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);

        
        copy_logic(connFd, outFd);
        close(connFd);
    }

    return 0;
}
