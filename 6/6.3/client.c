#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_LEN 1024

int main(int argc, char **argv) {
    int sockfd,
        n, len, clilen, servlen;
    char sendline[BUF_LEN], recvline[BUF_LEN];
    struct sockaddr_in servaddr, cliaddr;

    if (argc < 4) {
        printf("./client <IP address> <client port> <server port>\n");
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("client started\n");
    clilen = sizeof cliaddr;
    servlen = sizeof servaddr;

    memset(&cliaddr, 0, clilen);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(atol(argv[2]));
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *) &cliaddr, clilen) == -1) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atol(argv[3]));
    // if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
    //     printf("Invalid IP address\n");
    //     close(sockfd); /* По окончании работы закрываем дескриптор сокета */
    //     exit(1);
    // }

    printf("Введите сообщение: ");
    fgets(sendline, BUF_LEN, stdin);

    if(sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        perror("send failed");
        close(sockfd);
        exit(1);
    }

    /* Ожидаем ответа и читаем его. Максимальная
    допустимая длина датаграммы – 1000 символов,
    адрес отправителя нам не нужен */
    // if((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *) NULL, NULL)) < 0)
    // {
    //     perror(NULL);
    //     close(sockfd);
    //     exit(1);
    // }

    /* Печатаем пришедший ответ и закрываем сокет */
    // printf("from server: %s\n", recvline);
    close(sockfd);
    return 0;
}
