#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: ;
            srand(time(NULL));
            int a;
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                a = rand() % 1000;
                write(pipefd[1], &a, sizeof(int));
            }
            exit(EXIT_SUCCESS);
        default: ;
            int b;
            FILE *fp = fopen("out", "w");
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                read(pipefd[0], &b, sizeof(int));
                fprintf(fp, "from pipe: %d\n", b);
                
                printf("from pipe = %d\n", b);
            }
            fclose(fp);
            break;
    }
    exit(EXIT_SUCCESS);
}
