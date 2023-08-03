// TCP SERVER

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "erproc.h"

#define SIZE 1024

int main(int argc, char *argv[])
{
    char buf[SIZE]; // Буфер для различных нужд
    int sockfd, newsockfd; // дескрипторы сокетов
    int serv_port; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    printf("Запущен TCP-сервер\n");

    if (argc < 2) {
        fprintf(stderr, "%s <server port>\n", argv[0]);
        exit(1);
    }

    serv_port = atoi(argv[1]);

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(serv_port);
    Bind(sockfd, (struct sockaddr *) &serv_addr, sizeof serv_addr);

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = Accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    Sendto(newsockfd, "hello from server\n", 18, 0, (struct sockaddr *) &cli_addr, sizeof cli_addr);

    FILE *fp = fopen("target.txt", "w");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int n;

    while(1)
    {
        n = recvfrom(newsockfd, buf, SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
        if (n <= 0)
        {
            break;
        }
        printf("rcv %s", buf);
        fprintf(fp, "%s", buf);
        bzero(buf, SIZE);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
