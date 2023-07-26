#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <wait.h>

int child_forked = 0;
int stop_read = 0;
pid_t pid1, pid2;

void check_child(int signo)
{
    printf("check_child\n");
    if (!child_forked && signo == SIGUSR1)
    {
        child_forked = 1;
    }

}

void handler(int signo)
{
    printf("handler (pid1 = %d, pid2 = %d, pid = %d)\n", pid1, pid2, getpid());
    if (!child_forked && signo == SIGUSR1)
    {
        printf("check_child\n");
        child_forked = 1;
    }
    else
    {
        switch(signo)
        {
            case SIGUSR1:
                printf("sigusr1");
                stop_read = 1;
                break;
            case SIGUSR2:
                printf("sigusr2");
                stop_read = 0;
                break;
            default:
                printf("other signal");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Введите количество случайных чисел первым аргументом.\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    pid1 = getpid();
    pid2 = fork();

    switch(pid2)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            // signal(SIGUSR1, handler);
            // signal(SIGUSR2, handler);
            kill(pid1, SIGUSR1);  // Сигнал по запуску дочернего процесса
            child_forked = 1;

            while (stop_read) ;

            exit(EXIT_SUCCESS);
        default:
            // signal(SIGUSR1, check_child);
            while(!child_forked) ;  // Одижание запуска дочернего процесса

            kill(pid2, SIGUSR1);

            kill(pid2, SIGUSR2);
    }
    exit(EXIT_SUCCESS);
}
