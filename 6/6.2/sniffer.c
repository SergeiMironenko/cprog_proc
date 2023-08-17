#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF_LEN 65536
FILE *fp = NULL;  // Указатель на файл
static int while_exec = 1;  // Выполнение


void print_data(unsigned char *buf , int size);
void print_ip_header(char *buf, int size);
void print_udp_header(char *buf, int size);
void print_udp_packet(char *buf , int size);
void sig_handler(int signo);


int main() {
    int sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock_raw == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    uint8_t buf[BUF_LEN];
    struct sockaddr_in addr_in;
    socklen_t len = sizeof addr_in;

    fp = fopen("dump", "w");
    if (fp == NULL) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sig_handler);

    while (while_exec) {
        ssize_t n = recvfrom(sock_raw, buf, BUF_LEN, 0, (struct sockaddr *) &addr_in, &len);
        if (n > 0) {
            print_udp_packet(buf, n);
            printf("packet printed to dump file\n");
        }
        sleep(1);
    }

    fclose(fp);
    printf("exiting...\n");
    exit(EXIT_SUCCESS);
}


void print_data(unsigned char *buf , int size)
{
    // Вывод данных по 16 октетов в строке
    for (int i = 0; i < size; i++){

        // Выписаны 16 чисел - нужен отступ и читабельный вид данных
        if (i != 0 && i % 16 == 0)  {

            // Отступ между байтами и символами
            fprintf(fp,"         ");

            // Символы
            for (int j = i - 16; j < i; j++) {
                if (buf[j] >= 32 && buf[j] <= 128)
                    fprintf(fp, "%c", buf[j]);  // Вывод символа алфавита
                else
                    fprintf(fp, ".");  // Либо точки
            }

            fprintf(fp, "\n");
        }

        // Отступ перед строкой
        if (i % 16 == 0) fprintf(fp,"   ");

        // Вывод байта в 16 форме
        fprintf(fp, " %02X", buf[i]);

        // Если конец последовательности
        if (i == size - 1) {

            // Дополнительные пробелы
            for (int j = 0; j < 15 - i % 16; j++) fprintf(fp,"   ");

            fprintf(fp,"         ");

            for (int j = i - i % 16; j <= i; j++) {
                if (buf[j] >= 32 && buf[j] <= 128) fprintf(fp, "%c", buf[j]);
                else fprintf(fp, ".");
            }

            fprintf(fp, "\n");
        }
    }
}


void print_ip_header(char* buf, int size)
{
    struct sockaddr_in source, dest;

    struct iphdr *iph = (struct iphdr *)buf;  // Заголовок IP
    unsigned int iphdrlen = ntohs(iph->ihl) * 4;  // Длина IP - заголовка

    memset(&source, 0, sizeof(source));  // Заполнение source нулями
    source.sin_addr.s_addr = iph->saddr;  // Адрес источника

    memset(&dest, 0, sizeof(dest));  // Заполнение source нулями
    dest.sin_addr.s_addr = iph->daddr;  // Адрес назначения

    // ntohs используется тольков полях >= 2 байта
    // inet_ntoa для отображения адреса в строке
    fprintf(fp,"\n");
    fprintf(fp,"IP Header\n");
    fprintf(fp,"   |-IP Version        : %d\n", iph->version);
    fprintf(fp,"   |-IP Header Length  : %d Bytes\n", iphdrlen);
    fprintf(fp,"   |-Type Of Service   : %d\n", iph->tos);
    fprintf(fp,"   |-IP Total Length   : %d Bytes\n", ntohs(iph->tot_len));
    fprintf(fp,"   |-Identification    : %d\n", ntohs(iph->id));
    fprintf(fp,"   |-TTL               : %d\n", iph->ttl);
    fprintf(fp,"   |-Protocol          : %d\n", iph->protocol);
    fprintf(fp,"   |-Checksum          : %d\n", ntohs(iph->check));
    fprintf(fp,"   |-Source IP         : %s\n", inet_ntoa(source.sin_addr));
    fprintf(fp,"   |-Destination IP    : %s\n", inet_ntoa(dest.sin_addr));
}


void print_udp_header(char *buf, int size)
{
    struct udphdr *udph = (struct udphdr *)buf;  // Заголовок UDP
    fprintf(fp, "\n");
    fprintf(fp, "UDP Header\n");
    fprintf(fp, "   |-Source Port       :%d\n", ntohs(udph->source));
    fprintf(fp, "   |-Destination Port  :%d\n", ntohs(udph->dest));
    fprintf(fp, "   |-Length            :%d\n", ntohs(udph->len));
    fprintf(fp, "   |-Checksum          :%d\n", ntohs(udph->check));
}


void print_udp_packet(char *buf , int size)
{
    struct iphdr *iph = (struct iphdr *)buf;  // Заголовок IP
    unsigned int iphdrlen = (unsigned short)(iph->ihl * 4);  // Длина заголовка (5 строк * 4 байта в каждой)
    struct udphdr *udph = (struct udphdr*)(buf + iphdrlen);  // Заголовок UDP

    fprintf(fp,"\n\n***********************UDP Packet*************************\n");

    print_ip_header(buf,size);
    print_udp_header(buf + iphdrlen, size);

    fprintf(fp,"\n");
    fprintf(fp,"IP Header\n");
    print_data(buf, iphdrlen);

    fprintf(fp,"UDP Header\n");
    print_data(buf + iphdrlen, sizeof udph);

    fprintf(fp,"Data Payload\n");
    print_data(buf + iphdrlen + sizeof udph, (size - sizeof udph - iph->ihl * 4));

    fprintf(fp,"\n###########################################################");
    fprintf(fp,"\n");
}


void sig_handler(int signo) {
    if (signo == SIGINT) while_exec = !while_exec;
}
