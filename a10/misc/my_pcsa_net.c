#include <asm-generic/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LISTENQ 5
int open_listenfd(char *port) {
    struct addrinfo hints;
    struct addrinfo *listp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
    int retCode = getaddrinfo(NULL, port, &hints, &listp);

    if (retCode < 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(retCode));
        exit(-1);
    }

    int listenFd;
    struct addrinfo *p;
    for (p=listp;p!=NULL;p=p->ai_next) {
        if ((listenFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) continue;

        int optVal = 1;
        setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optVal, sizeof(int));

        if (bind(listenFd, p->ai_addr, p->ai_addrlen) == 0)
            break;

        close(listenFd);
    }

    freeaddrinfo(listp);

    if (!p) return -1;

    if (listen(listenFd, LISTENQ) < 0) {
        close(listenFd);
        return -1;
    }

    return listenFd;
}

int open_clientfd(char *hostname, char *port) {
    struct addrinfo hints;
    struct addrinfo* listp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;
    int retCode = getaddrinfo(hostname, port, &hints, &listp);

    if (retCode < 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(retCode));
        exit(-1);
    }

    int clientFd;
    struct addrinfo *p;
    for (p=listp;p!=NULL;p->ai_next) {
        if ((clientFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;

        if (connect(clientFd, p->ai_addr, p->ai_addrlen) != -1)
            break;

        close(clientFd);
    }

    freeaddrinfo(listp);

    if (!p) return -1;

    return clientFd;
}
