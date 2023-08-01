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

#define MAX_CLIENTS 3

int a = 3;
int b = 5;

// функция обслуживания
// подключившихся пользователей
void dostuff(int);

// функция обработки ошибок
void error(const char *msg) {
    perror(msg);
    exit(1);
}

// количество активных пользователей
int nclients = 0;

// печать количества активных
// пользователей
void printusers() {
    if(nclients) {
        printf("%d user on-line\n",
        nclients);
    }
    else {
        printf("No User on line\n");
    }
}

// функция обработки данных
int myfunc(int a, int b) {
    return a + b;
}

int main(int argc, char *argv[])
{
    char buff[1024]; // Буфер для различных нужд
    int sockfd, newsockfd; // дескрипторы сокетов
    int serv_port; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    printf("Запущен TCP-сервер\n");

    // ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    serv_port = atoi(argv[1]);

    // Шаг 1 - создание сокета
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(serv_port);
    Bind(sockfd, (struct sockaddr *) &serv_addr, sizeof serv_addr);

    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    while (1)
    {
        // Для единственного клиента
        if (nclients == 0)
        {
            newsockfd = Accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            Sendto(newsockfd, "hello from server\n", 18, 0, (struct sockaddr *) &cli_addr, sizeof cli_addr);
            nclients++;

            // вывод сведений о клиенте
            struct hostent *hst;
            hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
            int port = ntohs(cli_addr.sin_port);
            printf("+%s [%s] new connect! (%d, port = %d)\n",
            (hst) ? hst->h_name : "Unknown host",
            (char*)inet_ntoa(cli_addr.sin_addr),
            nclients,
            port);
            printusers();
        }
        int n = Recvfrom(newsockfd, buff, 1000, 0, (struct sockaddr *) &cli_addr, &clilen);
        printf("buff = %s\n", buff);

        buff[n] = 0;
        switch(buff[0])
        {
            case '+':
                printf("%d + %d = %d\n", a, b, a + b);
                break;
            case '-':
                printf("%d - %d = %d\n", a, b, a - b);
                break;
            case '*':
                printf("%d * %d = %d\n", a, b, a * b);
                break;
        }
    }
    pid = fork();
    if (pid < 0) error("ERROR on fork");
    if (pid == 0) {
        close(sockfd);
        dostuff(newsockfd);
        exit(0);
    }
    else close(newsockfd);
    close(sockfd);
    return 0;
}
