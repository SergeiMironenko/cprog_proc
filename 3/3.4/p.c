#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void listener(int signo)
{
    switch(signo)
    {
        case SIGINT:
            printf("SIGINT");
            break;
        case SIGQUIT:
            printf("SIGQUIT");
            break;
    }
    exit(EXIT_SUCCESS);
}

int main()
{
    int i = 0;
    signal(SIGINT, listener);
    signal(SIGQUIT, listener);

    while (1)
    {
        FILE *fp = fopen("out.txt", "a");
        if (fp == NULL)
        {
            perror("fopen");
            exit(EXIT_SUCCESS);
        }

        fprintf(fp, "%d\n", i);
        fclose(fp);

        i++;
        sleep(1);
    }
}
