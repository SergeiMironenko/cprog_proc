#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int i = 0;
    signal(SIGINT, SIG_IGN);

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
