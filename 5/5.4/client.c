// TCP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "erproc.h"

#define SIZE 1024

int main(int argc, char *argv[]) {
    int my_sock, serv_port, cli_port, n;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *hent;
    char buf[SIZE];
    printf("Запущен TCP-клиент\n");

    if (argc < 4) {
        fprintf(stderr, "%s <hostname> <server port> <client port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hent = gethostbyname(argv[1]);
    if (hent == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(EXIT_FAILURE);
    }
    printf("hostname = %s\n", hent->h_name);

    serv_port = atoi(argv[2]);
    cli_port = atoi(argv[3]);

    // заполнение структуры serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    Inet_aton(hent->h_name, &serv_addr.sin_addr);

    my_sock = Socket(AF_INET, SOCK_STREAM, 0);

    bzero((char*) &cli_addr, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(cli_port);
    Bind(my_sock, (struct sockaddr *) &cli_addr, sizeof cli_addr);

    Connect(my_sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr));

    FILE *fp = fopen("source.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while(fgets(buf, SIZE, fp) != NULL)
    {
        Send(my_sock, buf, sizeof(buf), 0);
        printf("send %s", buf);
        bzero(buf, SIZE);
    }

    close(my_sock);
    exit(EXIT_SUCCESS);
}
