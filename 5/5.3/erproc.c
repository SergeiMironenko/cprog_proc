#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "erproc.h"

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if(res == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("connect failed");
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

void Send(int sockfd, const void *buf, size_t len, int flags)
{
    int res = send(sockfd, buf, len, flags);
    if (res == -1)
    {
        perror("send failed");
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

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    ssize_t res = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (res == -1)
    {
        if (errno == EAGAIN)
        {
            // 
        }
        else
        {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
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

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int res = accept(sockfd, addr, addrlen);
    if (res == -1)
    {
        if (errno == EAGAIN)
        {
            // accept имеет O_NONBLOCK и соединение не установлено
        }
        else
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
    }
    return res;
}
