#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    int rv;
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            for (int i = argc / 2 + 1; i < argc; i++)
            {
                printf("%d^2 = %d (child)\n", i, atoi(argv[i]) * atoi(argv[i]));
            }
            exit(EXIT_SUCCESS);
        default:
            for (int i = 1; i < argc / 2 + 1; i++)
            {
                printf("%d^2 = %d (parent)\n", i, atoi(argv[i]) * atoi(argv[i]));
            }
            wait(&rv);
            printf("child status: %d\n", rv);
    }
    exit(EXIT_SUCCESS);
}
