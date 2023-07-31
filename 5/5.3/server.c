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
    int portno; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    printf("TCP SERVER DEMO\n");

    // ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Шаг 1 - создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        nclients++;

        sendto(newsockfd, "hello from server\n", 18, 0, (struct sockaddr *) &cli_addr, sizeof cli_addr);
        int n = recv(newsockfd, buff, 1000, 0);
        buff[n] = 0;
        switch(buff[0])
        {
            case '+':
                printf("3 + 5 = %d\n", a + b);
                break;
            case '-':
                printf("3 - 5 = %d\n", a - b);
                break;
            case '*':
                printf("3 * 5 = %d\n", a * b);
                break;
        }
        printf("buff = %s\n", buff);

        // вывод сведений о клиенте
        struct hostent *hst;
        hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
        printf("+%s [%s] new connect!\n",
        (hst) ? hst->h_name : "Unknown host",
        (char*)inet_ntoa(cli_addr.sin_addr));
        printusers();
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