#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void show_pipe(int pipefd[2], char idx)
{
    char buf[100];
    read(pipefd[0], buf, 100);
    printf("buf (%d) = %s\n", idx, buf);
}

void swap_pipes(int pipefd1[2], int pipefd2[2])
{
    int tmp[2];
    tmp[0] = pipefd1[0];
    tmp[1] = pipefd1[1];
    pipefd1[0] = pipefd2[0];
    pipefd1[1] = pipefd2[1];
    pipefd2[0] = tmp[0];
    pipefd2[1] = tmp[1];
}

int main()
{
    int pipefd[3][2];
    pid_t pid;

    pipe(pipefd[0]);
    pipe(pipefd[1]);
    // pipe(pipefd[2]);

    // switch(pid = fork())
    // {
    //     case 0:
    //         close(pipefd[0][0]);
    //         close(pipefd[1][0]);
    //         write(pipefd[0][1], "asd", 4);
    //         write(pipefd[1][1], "qwe4", 5);
    //         exit(EXIT_SUCCESS);
    //     default:
    //         swap_pipes(pipefd[0], pipefd[1]);

    //         show_pipe(pipefd[0], 1);
    //         show_pipe(pipefd[1], 2);
    // }

    pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(pipefd[1][0]);
            close(pipefd[0][1]);
            close(pipefd[0][0]);
            dup2(pipefd[1][1], STDOUT_FILENO);
            execlp("ls", "ls", NULL);
        default:
            pid = fork();
            switch(pid)
            {
                case -1:
                    perror("fork2");
                    exit(EXIT_FAILURE);
                case 0: ;
                    swap_pipes(pipefd[0], pipefd[1]);
                    close(pipefd[0][1]);
                    close(pipefd[1][0]);
                    // close(pipefd[1][1]);
                    dup2(pipefd[0][0], STDIN_FILENO);
                    dup2(pipefd[1][1], STDOUT_FILENO);
                    execlp("wc", "wc", NULL);
                default:
                    pid = fork();
                    switch(pid)
                    {
                        case -1:
                            perror("fork3");
                            exit(EXIT_FAILURE);
                        case 0:
                            close(pipefd[0][1]);
                            close(pipefd[1][1]);
                            show_pipe(pipefd[0], 1);
                            show_pipe(pipefd[1], 2);
                            // swap_pipes(pipefd[0], pipefd[1]);
                            // close(pipefd[0][1]);
                            // close(pipefd[1][1]);
                            // close(pipefd[0][0]);
                            // dup2(pipefd[0][0], STDIN_FILENO);
                            // execlp("wc", "wc", NULL);
                            exit(EXIT_SUCCESS);
                        default:
                            // pid = fork();
                            // switch(pid)
                            // {
                            //     case -1:
                            //         perror("fork4");
                            //         exit(EXIT_FAILURE);
                            //     case 0:
                            //         close(pipefd[0][1]);
                            //         close(pipefd[1][1]);
                            //         close(pipefd[0][0]);
                            //         dup2(pipefd[1][0], STDIN_FILENO);
                            //         execlp("head", "head", "-c", "22", NULL);
                            //     default:
                            //         break;
                            // }
                            break;
                    }
            }
    }

    exit(EXIT_SUCCESS);
}
