// Дописывает в файл строку.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define STR_LEN 30

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (fd == -1)
    {
        exit(EXIT_FAILURE);
    }

    char buf[STR_LEN];
    printf("Введите строку: ");
    fgets(buf, STR_LEN, stdin);

    write(fd, buf, strlen(buf));
    close(fd);

    exit(EXIT_SUCCESS);
}
