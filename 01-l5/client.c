//
// Created by yingcheng.zhou on 2019-11-05.
//

#include <arpa/inet.h>
#include "../common.h"

#define MESSAGE_SIZE 102400

void send_data(int);

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &servaddr.sin_addr);
    int connect_rt = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (connect_rt < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    send_data(sockfd);
    return 0;
}

void send_data(int sockfd) {
    char *query;
    query = malloc(MESSAGE_SIZE+1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        int n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send in to buffer %d \n", n_written);
        if (n_written <= 0) {
            perror("send");
            return;
        }

        remaining -= n_written;
        cp += n_written;
    }
}