// UDP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "fun.h"

#define BUF_LEN 256

struct thread_args
{
    int sockfd;
};

void reader(void *args)
{
    struct thread_args *targs = args;
    char buf[BUF_LEN];
    while (1)
    {
        Recvfrom(targs->sockfd, buf, BUF_LEN, 0, (struct sockaddr *) NULL, NULL);
        printf("\nReceived from server: %s>", buf);
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("./client <IP address> <server port> <client port>\n");
        exit(EXIT_FAILURE);
    }

    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in cliaddr = {0};
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(atol(argv[3]));

    Bind(sockfd, (struct sockaddr *) &cliaddr, sizeof cliaddr);

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atol(argv[2]));
    Inet_aton(argv[1], &servaddr.sin_addr);

    char buf[BUF_LEN];

    pthread_t thread_id;
    struct thread_args args;
    args.sockfd = sockfd;
    pthread_create(&thread_id, NULL, (void *) reader, (void *) &args);

    while (1)
    {
        printf(">");
        fgets(buf, BUF_LEN, stdin);
        Sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *) &servaddr, sizeof servaddr);
    }

    close(sockfd);
    return 0;
}
