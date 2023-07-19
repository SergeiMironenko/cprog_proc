#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARGS_LEN 10

void qqq()
{
    char argv[3][10];
    scanf("%s", argv[0]);
    scanf("%s", argv[1]);
    // scanf("%s", argv[2]);
    printf("%s", argv[0]);
    printf("%s", argv[1]);
    // printf("%d", argv[2][0]);
}

int main(int argc, char *argv[])
{
    // char s[ARGS_LEN];
    // printf("Введите имя и аргументы программы: ");
    // fgets(s, ARGS_LEN, stdin);
    // printf("s = %s\n", s);
    qqq();
    // execl("sum_argv", "sum_argv", "2", "3", "1", "4", NULL);
    return 0;
}
