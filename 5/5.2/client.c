// UDP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fun.h"

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

    int n, len;
    char sendline[1000], recvline[1000];

    while (1)
    {
        printf("String => ");
        fgets(sendline, 1000, stdin);

        Sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &servaddr, sizeof servaddr);
        Recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) NULL, NULL);

        printf("%s\n", recvline);
    }

    close(sockfd);
    return 0;
}
