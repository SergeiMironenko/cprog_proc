#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

#define MAX_ARG 20  // Максимальное количество аргументов
#define MAX_PIPE 20  // Максимальное количество каналов

// Ввод аргументов
int get_args(char *s[MAX_ARG], int *n)
{
    char c = 0;
    *n = 0;
    while (c != '\n' && *n < MAX_ARG)
    {
        s[*n] = (char*)malloc(sizeof(char));
        scanf("%s%c", s[*n], &c);
        (*n)++;
    }
    s[*n] = NULL;
    return 0;
}

int main(int argc, char *argv[])
{
    char *args[MAX_ARG];

    // Количество аргументов (не считая NULL в конце)
    int n = 0;

    pid_t pid;
    int status = 0;

    printf("Введите имя и аргументы программы: ");
    get_args(args, &n);

    // Инициализация каналов
    int pipefd[MAX_PIPE][2];
    for(int i = 0; i < MAX_PIPE; i++)
    {
        pipe(pipefd[i]);
    }

    int argi = 0;  // Текущий аргумент
    int cmdi = 0;  // Аргумент, на котором начинается команда (самый первый и те, которые идут после |)
    int pipei = 0;  // Текущий канал

    while(argi < MAX_ARG)
    {
        // Конец последовательности
        if (args[argi] == NULL)
        {
            // Если аргумент единственный
            if (cmdi == 0)
            {
                switch(pid = fork())
                {
                    case -1:
                        perror("fork");
                        exit(EXIT_SUCCESS);
                    case 0:
                        // printf("child first and last\n");
                        execvp(args[cmdi], &args[cmdi]);
                }
            }

            // Если аргумент последний
            else
            {
                switch(pid = fork())
                {
                    case -1:
                        perror("fork");
                        exit(EXIT_SUCCESS);
                    case 0:
                        // printf("child last\n");
                        close(pipefd[pipei - 1][1]);
                        dup2(pipefd[pipei - 1][0], STDIN_FILENO);
                        execvp(args[cmdi], &args[cmdi]);
                }
            }

            pipei++;
            cmdi = argi + 1;

            break;
        }
        else if (strcmp(args[argi], "|") == 0)
        {
            args[argi] = NULL;

            // Если аргумент первый
            if (cmdi == 0)
            {
                switch(pid = fork())
                {
                    case -1:
                        perror("fork");
                        exit(EXIT_SUCCESS);
                    case 0:
                        // printf("child first\n");
                        close(pipefd[pipei][0]);
                        dup2(pipefd[pipei][1], STDOUT_FILENO);
                        execvp(args[cmdi], &args[cmdi]);
                }
            }

            // Если аргумент в середине
            else
            {
                switch(pid = fork())
                {
                    case -1:
                        perror("fork");
                        exit(EXIT_SUCCESS);
                    case 0:
                        // printf("child\n");
                        close(pipefd[pipei][0]);
                        dup2(pipefd[pipei][1], STDOUT_FILENO);
                        close(pipefd[pipei - 1][1]);
                        dup2(pipefd[pipei - 1][0], STDIN_FILENO);
                        execvp(args[cmdi], &args[cmdi]);
                }
            }

            pipei++;
            cmdi = argi + 1;
        }

        argi++;
    }

    // Освобождение аргументов
    for (int i = 0; i < n; i++)
    {
        free(args[i]);
    }

    exit(EXIT_SUCCESS);
}
