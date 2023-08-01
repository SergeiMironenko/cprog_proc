// UDP SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fun.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("./server <port>\n");
        exit(EXIT_FAILURE);
    }

    int server = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atol(argv[1]));
    Bind(server, (struct sockaddr *) &servaddr, sizeof servaddr);

    int clilen, n;
    char line[1000];
    struct sockaddr_in cliaddr = {0};

    struct sockaddr_in c_addr[2];
    int c_port[2];
    int clients = 0;

    while(1)
    {
        clilen = sizeof cliaddr;
        Recvfrom(server, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen);
        if (clients == 0)
        {
            c_addr[0] = cliaddr;
            c_port[0] = ntohs(c_addr[0].sin_port);
            clients++;
            printf("client 0 (port %d) was connected\n", c_port[0]);
            char mod_line[1000] = "You are the only client: ";
            strcat(mod_line, line);
            Sendto(server, mod_line, strlen(mod_line) + 1, 0, (struct sockaddr *) &cliaddr, clilen);
        }
        else if (clients == 1)
        {
            if (c_port[0] == ntohs(cliaddr.sin_port))
            {
                printf("one client\n");
                char mod_line[1000] = "You are the only client: ";
                strcat(mod_line, line);
                Sendto(server, mod_line, strlen(mod_line) + 1, 0, (struct sockaddr *) &cliaddr, clilen);
            }
            else
            {
                c_addr[1] = cliaddr;
                c_port[1] = ntohs(c_addr[1].sin_port);
                clients++;
                printf("client 1 (port %d) was connected\n", c_port[1]);
                char mod_line[1000] = "From client 1: ";
                strcat(mod_line, line);
                Sendto(server, mod_line, strlen(mod_line) + 1, 0, (struct sockaddr *) &c_addr[0], sizeof c_addr[0]);
            }
        }
        else
        {
            if (c_port[0] == ntohs(cliaddr.sin_port))
            {
                char mod_line[1000] = "From client 0: ";
                strcat(mod_line, line);
                Sendto(server, mod_line, strlen(mod_line) + 1, 0, (struct sockaddr *) &c_addr[1], sizeof c_addr[1]);
            }
            else if (c_port[1] == ntohs(cliaddr.sin_port))
            {
                char mod_line[1000] = "From client 1: ";
                strcat(mod_line, line);
                Sendto(server, mod_line, strlen(mod_line) + 1, 0, (struct sockaddr *) &c_addr[0], sizeof c_addr[0]);
            }
        }
        printf("(client port %d): %s\n", ntohs(cliaddr.sin_port), line);
    }
    return 0;
}
