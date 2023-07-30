// UDP SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include "fun.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("./server <port>\n");
        exit(EXIT_FAILURE);
    }

    int server = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atol(argv[1]));
    Bind(server, (struct sockaddr *) &servaddr, sizeof servaddr);

    int clilen, n;
    char line[1000];
    struct sockaddr_in cliaddr = {0};

    while(1)
    {
        clilen = sizeof cliaddr;
        Recvfrom(server, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s from %d\n", line, ntohs(cliaddr.sin_port));
        Sendto(server, line, strlen(line) + 1, 0, (struct sockaddr *) &cliaddr, clilen);
    }
    return 0;
}
