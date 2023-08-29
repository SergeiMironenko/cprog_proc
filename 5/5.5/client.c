#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_PORT 1234
// #define CLIENT_PORT 2345
#define SERVER_ADDR "127.0.0.1"
// #define CLIENT_ADDR "127.0.0.3"
#define BUF_LEN 80

int cli_sock, serv_sock;

int main(int argc, char *argv[]) {
    socklen_t cli_len, serv_len;
    struct sockaddr_in cli_addr, serv_addr;
    int fd;
    char buf[BUF_LEN];
    ssize_t recv_size;
    int cli_port;

    if (argc < 3) {
        printf("Input format: ./client <addr> <port>\n");
        return 1;
    }
    cli_port = atoi(argv[2]);

    if ((cli_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return 1;
    }

    printf("Client socket created\n");

    cli_len = sizeof(cli_addr);
    serv_len = sizeof(serv_addr);

    memset(&cli_addr, 0, cli_len);
    cli_addr.sin_addr.s_addr = inet_addr(argv[1]);
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(cli_port);

    if (bind(cli_sock, (struct sockaddr *) &cli_addr, cli_len) == -1) {
        perror("bind failed\n");
        return 1;
    }

    memset(&serv_addr, 0, serv_len);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (connect(cli_sock, (struct sockaddr *) &serv_addr, serv_len) == -1) {
        perror("connect failed");
        return 1;
    }

    printf("Client socket connected\n");

    bzero(buf, BUF_LEN);
    while (1) {
        recv_size = recv(cli_sock, buf, BUF_LEN, 0);
        if (recv_size == 10 && strlen(buf) == 0) break;
        printf("Received: %ld bytes\n", recv_size);
        // printf("Buf: %s\n", buf);
        bzero(buf, BUF_LEN);
    }

    close(cli_sock);
    return 0;
}
