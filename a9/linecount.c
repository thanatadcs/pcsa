/*
Part I:
Question: How does BUFSIZE affect copy performance?
Answer: The larger the BUFSIZE the better copy performance. Say BUFSIZE of 1 byte copy run very slow compare to 100 MiB of BUFSIZE. (BUFSIZE too large is not good either, see answer below).

Question: What value should we use for BUFSIZE?
Answer: For this I think we can try to benchmark the performance with respect to different BUFSIZE, but if the BUFSIZE get too large (like 1GB), then it might be too much memory consumption (this might affect performance too). So I think in this case it is a trade-off between memory usage and I/O performance, larger BUFSIZE means less system calls, but also mean more memory consumption. Generally, I think setting BUFSIZE to be power of 2 and/or the multiple of block size might be a good idea. (Please correct me and advice me more on this if I'm wrong)
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#ifndef BUFSIZE
#define BUFSIZE 512
#endif

int fd;

int count_newline(const char *s, ssize_t size) {
    int to_return = 0;
    for (int i=0;i<size;i++) {
        if (!isprint(s[i]) && s[i] != '\n') {
            fprintf(stderr, "Binary file. Bye.\n");
            close(fd);
            exit(1);
        }
        to_return += s[i] == '\n';
    }
    return to_return;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "./linecount <filename>\n");
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(0);
        return 1;
    }

    char buf[BUFSIZE];
    ssize_t read_num;
    int count = 0;
    while ((read_num = read(fd, &buf, BUFSIZE))) {
        count += count_newline(buf, read_num);
    }
    printf("%d %s\n", count, argv[1]);
    close(fd);
    return 0;
}
