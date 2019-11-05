//
// Created by yingcheng.zhou on 2019-11-05.
//

#include "../common.h"

int make_sock(uint16_t port);
int readn(int, void *, size_t);
void read_data(int);

int main() {
    int sockfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    sockfd = make_sock(SERVER_PORT);
    listen(sockfd, 1024);
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
        read_data(connfd);
        close(connfd);
    }
    return 0;
}

int make_sock(uint16_t port) {
    int sockfd;
    struct sockaddr_in serveraddr;

    // create a TCP socket fd
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("init socket");
        exit(EXIT_FAILURE);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // bind port and ip
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        perror("bind port");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int remaining = (int) size;

    while (remaining > 0) {
        int n_read = read(fd, buffer_pointer, remaining);
        if (n_read < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if (n_read == 0) {
            break;
        }

        remaining -= n_read;
        buffer_pointer += n_read;
    }

    return (size - remaining);
}

void read_data(int connfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        n = readn(connfd, buf, 1024);
        if (n == 0) {
            return;
        }
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}