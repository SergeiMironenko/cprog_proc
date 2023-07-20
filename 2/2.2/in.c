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

    FILE *fd = fopen(argv[1], "a+");
    if (fd == NULL)
    {
        exit(EXIT_FAILURE);
    }

    char buf[STR_LEN];
    printf("Введите строку: ");
    fgets(buf, STR_LEN, stdin);

    fwrite(buf, sizeof(buf[0]), strlen(buf), fd);
    fclose(fd);

    exit(EXIT_SUCCESS);
}
