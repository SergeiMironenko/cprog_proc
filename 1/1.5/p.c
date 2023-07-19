#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

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
    
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            execvp(args[0], args);  // Отличие от 1.4: execvp вместо execv
            exit(EXIT_SUCCESS);
        default:
            for (int i = 0; i < n; i++)
            {
                free(args[i]);
            }
            wait(&rv);
            printf("Статус дочерней программы: %d\n", rv);
    }
}
