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

#define BUF_LEN 1024

void error(const char *msg) {
    perror(msg);
    exit(0);
}

struct thread_args
{
    int sock;
};

void reader(void *arg)
{
    struct thread_args *args = arg;
    int n;
    char buff[BUF_LEN];
    while ((n = recv(args->sock, &buff[0], sizeof(buff) - 1, 0)) > 0) {
        buff[n] = 0;
        printf("From hent: %s>", buff);
        fflush(stdout);
    }
}

int main(int argc, char *argv[]) {
    int my_sock, serv_port, cli_port, n;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *hent;
    char buff[1024];
    printf("Запущен TCP-клиент\n");

    if (argc < 4) {
        fprintf(stderr, "usage %s <hostname> <server port> <client port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hent = gethostbyname(argv[1]);
    if (hent == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(EXIT_FAILURE);
    }
    printf("hostname = %s\n", hent->h_name);

    // извлечение портов
    serv_port = atoi(argv[2]);
    cli_port = atoi(argv[3]);

    // заполнение структуры serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    Inet_aton(hent->h_name, &serv_addr.sin_addr);

    // Шаг 1 - создание сокета
    my_sock = Socket(AF_INET, SOCK_STREAM, 0);

    // заполнение структуры cli_addr
    bzero((char*) &cli_addr, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(cli_port);

    Bind(my_sock, (struct sockaddr *) &cli_addr, sizeof cli_addr);

    // Шаг 2 - установка соединения
    Connect(my_sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr));

    // Прием сообщений в отдельном потоке
    pthread_t thread;
    struct thread_args args;
    args.sock = my_sock;
    pthread_create(&thread, NULL, (void *) reader, (void *) &args);

    // Шаг 3 - чтение и передача сообщений
    while (1) {
        printf(">");
        fgets(buff, BUF_LEN, stdin);

        if (!strcmp(buff, "quit\n")) {
            // Корректный выход
            printf("Выход...");
            close(my_sock);
            return 0;
        }
        
        Send(my_sock, buff, strlen(buff), 0);
    }
    close(my_sock);
    exit(EXIT_SUCCESS);
}
