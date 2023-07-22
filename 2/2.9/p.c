#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

#define ARG_COUNT 10

// Ввод аргументов
int get_args(char *s[ARG_COUNT], int *n)
{
    char c = 0;
    *n = 0;
    while (c != '\n' && *n < ARG_COUNT - 2)
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
    char *args[ARG_COUNT];  // Вводимые аргументы
    int n = 0;  // Количество аргументов (не считая NULL в конце)
    int rv = 0;
    printf("Введите имя и аргументы программы: ");
    get_args(args, &n);

    int pipefd[2];
    pipe(pipefd);

    int k = 0;  // Индекс следующей команды
    for (int i = 0; i < n; i++)
    {
        char is_pipe = strcmp(args[i], "|") == 0;
        if (is_pipe || args[i + 1] == NULL)
        {
            if (is_pipe) args[i] = NULL;

            pid_t pid = fork();
            switch(pid)
            {
                case -1:
                    perror("fork");
                    exit(EXIT_FAILURE);
                case 0:
                    if (k == 0)
                    {
                        close(pipefd[0]);
                        dup2(pipefd[1], STDOUT_FILENO);
                    }
                    else
                    {
                        close(pipefd[1]);
                        dup2(pipefd[0], STDIN_FILENO);
                    }
                    execvp(args[k], &args[k]);
            }
            k = i + 1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        free(args[i]);
    }
}
