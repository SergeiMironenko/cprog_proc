#ifndef FUN_C
#define FUN_C

// #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <string.h>
// #include <stdio.h>
// #include <errno.h>
#include <unistd.h>
// #include <stdlib.h>
// #include <netdb.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t Sendto(int sockfd, void *buf, size_t len, int flags, struct sockaddr *dest_addr, socklen_t addrlen);

void Inet_aton(const char *cp, struct in_addr *inp);

#endif
