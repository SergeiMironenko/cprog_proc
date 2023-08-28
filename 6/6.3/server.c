#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/ip.h>
#include <linux/udp.h>

#define BUF_LEN 1024

int sockfd;
struct iphdr *iph;
struct udphdr *udph;

void sighandler(int signo) {
    printf("exiting by signal\n");
    close(sockfd);
    exit(EXIT_SUCCESS);
}

void print_data(unsigned char *buf , int size) {
    // Вывод данных по 16 октетов в строке
    for (int i = 0; i < size; i++) {

        // Выписаны 16 чисел - нужен отступ и читабельный вид данных
        if (i != 0 && i % 16 == 0) {

            // Отступ между байтами и символами
            fprintf(stdout,"         ");

            // Символы
            for (int j = i - 16; j < i; j++) {
                if (buf[j] >= 32 && buf[j] <= 128)
                    fprintf(stdout, "%c", buf[j]);  // Вывод символа алфавита
                else
                    fprintf(stdout, ".");  // Либо точки
            }

            fprintf(stdout, "\n");
        }

        // Отступ перед строкой
        if (i % 16 == 0) fprintf(stdout,"   ");

        // Вывод байта в 16 форме
        fprintf(stdout, " %02X", buf[i]);

        // Если конец последовательности
        if (i == size - 1) {

            // Дополнительные пробелы
            for (int j = 0; j < 15 - i % 16; j++) fprintf(stdout,"   ");

            fprintf(stdout,"         ");

            for (int j = i - i % 16; j <= i; j++) {
                if (buf[j] >= 32 && buf[j] <= 128) fprintf(stdout, "%c", buf[j]);
                else fprintf(stdout, ".");
            }

            fprintf(stdout, "\n");
        }
    }
}

void print_msg(char *buf, int size) {
    iph = (struct iphdr *) &buf;
    udph = (struct udphdr *) (&buf + iph->ihl * 4);
    print_data(buf + iph->ihl * 4 + sizeof udph, (size - sizeof udph - iph->ihl * 4));
}

int main(int argc, char *argv[]) {

    int clilen, servlen, n;
    char line[BUF_LEN];
    struct sockaddr_in servaddr, cliaddr;
    signal(SIGINT, sighandler);


    if (argc < 2) {
        printf("./server <port>\n");
        exit(EXIT_FAILURE);
    }

    printf("server started\n");
    servlen = sizeof servaddr;
    clilen = sizeof clilen;

    memset(&servaddr, 0, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atol(argv[1]));
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr *) &servaddr, servlen) == -1) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {

        if ((n = recvfrom(sockfd, line, BUF_LEN, 0, (struct sockaddr *) &cliaddr, &clilen)) == -1) {
            perror("recvfrom failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        print_msg(line, n);
    }

    close(sockfd);
    printf("exiting...\n");
    exit(EXIT_SUCCESS);
}
