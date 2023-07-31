#ifndef FUN_C
#define FUN_C

#include <arpa/inet.h>
#include <unistd.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t Sendto(int sockfd, void *buf, size_t len, int flags, struct sockaddr *dest_addr, socklen_t addrlen);

void Inet_aton(const char *cp, struct in_addr *inp);

#endif
