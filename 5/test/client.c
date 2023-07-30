#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "erproc.h"
#include <stdlib.h>

int main(int argc, char const* argv[])
{
    int fd = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    Connect(fd, (struct sockaddr *) &adr, sizeof adr);
    write(fd, "Hello\n", 6);
    char buf[256];
    ssize_t nread;
    nread = read(fd, buf, 256);
    if (nread == -1)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (nread == 0)
    {
        printf("eof");
    }
    write(STDOUT_FILENO, buf, nread);
    close(fd);
    return 0;
}
