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

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if(res == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = bind(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    ssize_t res = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (res == -1)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }
}

ssize_t Sendto(int sockfd, void *buf, size_t len, int flags, struct sockaddr *dest_addr, socklen_t addrlen)
{
    ssize_t res = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (res == -1)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
}

void Inet_aton(const char *cp, struct in_addr *inp)
{
    int res = inet_aton(cp, inp);
    if (res == 0)
    {
        perror("inet_aton failed");
        exit(EXIT_FAILURE);
    }
}
