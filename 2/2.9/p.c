#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

#define ARG_COUNT 10  // Максимальное количество аргументов
#define PIPE_COUNT 2

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
    // Реальное количество аргументов на 2 больше
    // Два дополнительных: имя программы (самый первый), NULL (послений)
    char *args[ARG_COUNT + 2];

    // Количество аргументов (не считая NULL в конце)
    // Для вывода аргументов в консоль или освобождения памяти
    int n = 0;

    pid_t pid;
    int rv = 0;

    printf("Введите имя и аргументы программы: ");
    get_args(args, &n);

    int pipefd[PIPE_COUNT][2];
    for(int i = 0; i < PIPE_COUNT; i++)
    {
        pipe(pipefd[i]);
    }

    for (int i = 0; i < n; i++)
    {
        free(args[i]);
    }
    exit(EXIT_SUCCESS);
}
