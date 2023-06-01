#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include "pcsa_net.h"

#define BUFSIZE 1024


int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "bad argument: ./send <serverAddr> <portNum> <inputFilename>\n");
        return -1;
    }

    int clientFd = open_clientfd(argv[1], argv[2]);
    if (clientFd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return -1;
    }
    
    int inFd = open(argv[3], O_RDONLY);
    if (inFd < 0) {
        fprintf(stderr, "Failed to open file\n");
        return -1;
    }
    ssize_t readNum;
    char buf[BUFSIZE];

    while ((readNum = read(inFd, buf, BUFSIZE))) {
        write_all(clientFd, buf, readNum);
    }

    close(clientFd);
    close(inFd);

    return 0;
}
