#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Введите количество случайных чисел первым аргументом.\n");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // FILE *fp;
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(pipefd[0]);
            srand(time(NULL));
            int a;
            char *buf = (char*)malloc(sizeof(char));
            FILE *fp2;
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                fp2 = fopen("out.txt", "r");
                fread(buf, sizeof(char), 1000, fp2);
                printf("buf from child = %s\n", buf);
                fclose(fp2);

                a = rand() % 1000;
                write(pipefd[1], &a, sizeof(int));
            }
            free(buf);
            exit(EXIT_SUCCESS);
        default:
            close(pipefd[1]);
            int b;

            kill(pid, SIGUSR1);

            FILE *fp = fopen("out.txt", "w");
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                read(pipefd[0], &b, sizeof(int));
                fprintf(fp, "from pipe: %d\n", b);
                printf("from pipe = %d\n", b);
            }
            fclose(fp);

            kill(pid, SIGUSR2);
    }
    exit(EXIT_SUCCESS);
}
