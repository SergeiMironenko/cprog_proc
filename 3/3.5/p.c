#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int i = 0;

    while (1)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        
        FILE *fp = fopen("out.txt", "a");
        if (fp == NULL)
        {
            perror("fopen");
            exit(EXIT_SUCCESS);
        }

        fprintf(fp, "%d\n", i);
        sleep(1);  // Увеличение времени работы с файлом
        fclose(fp);


        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        i++;
        sleep(1);
    }
}
