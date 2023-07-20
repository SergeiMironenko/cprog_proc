// Выводит содержимое файла
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(EXIT_FAILURE);
    }

    FILE *fd = fopen(argv[1], "r");
    if (fd == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    int off = ftell(fd);
    char *buf = (char*)malloc(off * sizeof(char));

    fseek(fd, 0, SEEK_SET);
    fread(buf, sizeof(char), off, fd);
    printf("%s", buf);

    free(buf);
    exit(EXIT_SUCCESS);
}
