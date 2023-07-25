#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int sigint_count = 0;

void listener()
{
    sigint_count++;
    if (sigint_count == 3) exit(EXIT_SUCCESS);
}

int main()
{
    int i = 0;
    signal(SIGINT, listener);

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
