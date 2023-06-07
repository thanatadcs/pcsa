#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "pcsa_net.h"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void respond_with_hello_world(int connFd) {
    char *msg = "<h1>Hello, World!!</h1>";
    char buf[BUFSIZE];
    sprintf(buf, "HTTP/1.1 200 OK\r\n"
    "Server: Tiny\r\n"
    "Content-length: %lu\r\n"
    "Connection: close\r\n"
    "Content-type: text/html\r\n\r\n", strlen(msg));
    write_all(connFd, buf, strlen(buf));
    write_all(connFd, msg, strlen(msg));
}

void respond_with_file(int connFd, char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror(0);
        return;
    }

    struct stat statbuf;
    char buf[BUFSIZE];

    // Get file info
    if (fstat(fd, &statbuf) == -1) {
        perror(0);
        return;
    }

    // Response and header
    sprintf(buf, "HTTP/1.1 200 OK\r\n"
    "Server: Tiny\r\n"
    "Content-length: %lu\r\n"
    "Connection: close\r\n"
    "Content-type: text/html\r\n\r\n", statbuf.st_size);
    write_all(connFd, buf, strlen(buf));

    // Body
    while (read(fd, buf, BUFSIZE)) {
        write_all(connFd, buf, BUFSIZE);
    }
}

void serve_http(int connFd) {
    char buf[BUFSIZE];

    if (!read_line(connFd, buf, BUFSIZE)) 
        return ;

    printf("LOG: %s\n", buf);

    char method[BUFSIZE], uri[BUFSIZE], version[BUFSIZE];
    sscanf(buf, "%s %s %s", method, uri, version);

    while (read_line(connFd, buf, BUFSIZE) > 0) {
        if (strcmp(buf, "\r\n") == 0) break;
    }

    if (strcmp(method, "GET") == 0 &&
            strcmp(uri, "/") == 0) {
        //respond_with_hello_world(connFd);
        respond_with_file(connFd, "./sample-www/index.html");
    }

}

int main(int argc, char* argv[]) { 
    // an fd for listening for incoming connn
    int listenFd = open_listenfd(argv[1]);

    for (;;) {
        struct sockaddr_storage clientAddr; // to store addr of the client
        socklen_t clientLen = sizeof(struct sockaddr_storage); // size of the above

        // ...gonna block until someone connects to our socket
        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA *) &clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0) 
            printf("Connection from %s:%s\n", hostBuf, svcBuf); 
        else 
            printf("Connection from UNKNOWN.");
        
        serve_http(connFd); // service the client on this fd
        close(connFd);
    }

    return 0;
}
