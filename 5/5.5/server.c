#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_PORT 1234
#define SERVER_ADDR "127.0.0.1"
#define BUF_LEN 80

int main() {
    int serv_sock, cli_sock;
    socklen_t serv_len, cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fp;
    char buf[BUF_LEN];

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return 1;
    }

    printf("Server socket created\n");

    serv_len = sizeof(serv_addr);
    cli_len = sizeof(cli_addr);

    bzero(&serv_addr, serv_len);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (bind(serv_sock, (struct sockaddr *) &serv_addr, serv_len) == -1) {
        perror("bind failed");
        return 1;
    }

    printf("Server socket bound\n");

    if (listen(serv_sock, 2) == -1) {
        perror("listen failed");
        return 1;
    }

    printf("Server socket listening\n");

    while (1) {
        cli_sock = accept(serv_sock, (struct sockaddr *) &cli_addr, &cli_len);

        if (cli_sock == -1) {
            printf("accept failed\n");
        }
        else {
            int pid = fork();
            switch(pid) {
                case -1:
                    perror("fork failed");
                    return 1;
                case 0:
                    printf("New client connected\n");
                    printf("Client address: %s\n", inet_ntoa(cli_addr.sin_addr));
                    printf("Client port: %d\n", ntohs(cli_addr.sin_port));

                    fp = fopen("file.txt", "r");
                    if (!fp) {
                        perror("open failed");
                        continue;
                    }

                    bzero(buf, BUF_LEN);
                    while (fgets(buf, BUF_LEN, fp)) {
                        send(cli_sock, buf, strlen(buf), 0);
                        bzero(buf, BUF_LEN);
                        sleep(3);
                    }

                    /*
                    * Для подтверждения конца файла отправляется пустое сообщение с несоответствующей длиной, например 10
                    */
                    send(cli_sock, "", 10, 0);

                    printf("File sent\n");
                    return 0;
            }
        }
    }

    close(serv_sock);
    return 0;
}
