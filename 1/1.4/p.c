#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define ARG_COUNT 10  // Максимальное количество аргументов

// Ввод аргументов
int get_args(char *s[ARG_COUNT], int *n)
{
    char c = 0;
    *n = 0;
    while (c != '\n' && *n <= ARG_COUNT)
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

    // Статус дочерней программы
    int rv = 0;

    printf("Введите имя и аргументы программы: ");
    get_args(args, &n);

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            execv(args[0], args);  // Отличие от 1.5: execv вместо execvp
            exit(EXIT_SUCCESS);
        default:
            wait(&rv);
            for (int i = 0; i < n; i++)
            {
                free(args[i]);
            }
            printf("Статус дочерней программы: %d\n", rv);
    }
}
