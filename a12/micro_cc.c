#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include "pcsa_net.h"

#define BUFSIZE 1024
typedef struct sockaddr SA;

struct survival_bag {
    struct sockaddr_storage cliendAddr;
    int connFd;
};

char *folder; // Input folder

void respond_with_file(int connFd, char *path) {
	char rel_path[BUFSIZE]; // Should be good enough for this
	strcpy(rel_path, folder);
	strcat(rel_path, path);

    int fd = open(rel_path, O_RDONLY);
    if (fd < 0) {
        perror(0);
		if (errno == ENOENT) {
			char *error_msg = "HTTP/1.1 404 Not Found\r\n\r\n";
			write_all(connFd, error_msg, strlen(error_msg));
		}
        return;
    }

    struct stat statbuf;
    char buf[BUFSIZE];

    // Get file info
    if (fstat(fd, &statbuf) == -1) {
        perror(0);
		close(fd);
        return;
    }

	// Check extension
	char *mime_type;
	char *dot_ptr = strrchr(path, '.');
	if (dot_ptr == NULL) {
		fprintf(stderr, "File extension not found\n");
		close(fd);
		return;
	}
	if (strcmp(dot_ptr, ".jpg") == 0 || strcmp(dot_ptr, ".jpeg") == 0) {
		mime_type = "image/jpeg";
	} else if (strcmp(dot_ptr, ".html") == 0) {
		mime_type = "text/html";
	} else {
		fprintf(stderr, "Incompatible file extension\n");
		close(fd);
		return;
	}

    // Response and header
    sprintf(buf, "HTTP/1.1 200 OK\r\n"
    "Server: Tiny\r\n"
    "Content-length: %ld\r\n"
    "Connection: close\r\n"
    "Content-type: %s\r\n\r\n", statbuf.st_size, mime_type);
    write_all(connFd, buf, strlen(buf));

    // Body
	int readNum;
    while ((readNum = read(fd, buf, BUFSIZE))) {
        write_all(connFd, buf, readNum);
    }
	
	close(fd);
}

void serve_http(int connFd) {
    char buf[BUFSIZE];

    if (!read_line(connFd, buf, BUFSIZE)) 
        return ;
    printf("LOG: %s\n", buf);

    char method[BUFSIZE], uri[BUFSIZE], version[BUFSIZE];
	if (sscanf(buf, "%s %s %s", method, uri, version) == 0) {
		fprintf(stderr, "Invalid input format\n");
		return ;
	}

    while (read_line(connFd, buf, BUFSIZE) > 0) {
        if (strcmp(buf, "\r\n") == 0) break;
    }

    if (strcmp(method, "GET") == 0 &&
			strlen(uri) > 0 &&
			strcmp(version, "HTTP/1.1") == 0) {
        //respond_with_hello_world(connFd);
        respond_with_file(connFd, uri);
    }
}

void *conn_handler(void *args) {
    struct survival_bag *context = (struct survival_bag *) args;

    pthread_detach(pthread_self());
    serve_http(context->connFd);
    close(context->connFd);
    free(context);
    return NULL;
}

int main(int argc, char* argv[]) { 

	if (argc <= 2 || argc >= 4) {
		fprintf(stderr, "./micro <portNum> <rootFolder>\n");
		return 1;
	}
    // an fd for listening for incoming connn
    int listenFd = open_listenfd(argv[1]);
	if (listenFd < 0) {
		fprintf(stderr, "Failed to listened on port %s\n", argv[1]);
		return 1;
	}
	folder = argv[2];

    for (;;) {
        struct sockaddr_storage clientAddr; // to store addr of the client
        socklen_t clientLen = sizeof(struct sockaddr_storage); // size of the above

        // ...gonna block until someone connects to our socket
        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);
		if (connFd < 0) {
			perror(0);
			continue;
		}

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA *) &clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0) 
            printf("Connection from %s:%s\n", hostBuf, svcBuf); 
        else 
            printf("Connection from UNKNOWN.");

        struct survival_bag *context =
            (struct survival_bag *) malloc(sizeof(struct survival_bag));
        context->connFd = connFd;
        memcpy(&context->cliendAddr, &clientAddr, sizeof(struct sockaddr_storage));

        pthread_t threadInfo;
        pthread_create(&threadInfo, NULL, conn_handler, (void *)context);
        //serve_http(connFd); // service the client on this fd
    }

    return 0;
}
