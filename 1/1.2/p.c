#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

void atexit_fun()
{
    printf("ok\n");
}

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    switch(pid)
    {
        case 0:
            printf("Дочерний процесс: argc = %d\n", argc);
            for (int i = 0; i < argc; i++) printf("argv[%d] = %s\n", i, argv[i]);
            break;
        default:
            printf("Родительский процесс: argc = %d\n", argc);
            for (int i = 0; i < argc; i++) printf("argv[%d] = %s\n", i, argv[i]);
            int atexit_status = atexit(atexit_fun);
            if (atexit_status)
            {
                printf("Ошибка регистрации функции atexit_fun\n");
            }
            break;
    }

    int rv = 0;
    wait(&rv);

    exit(EXIT_SUCCESS);
}
