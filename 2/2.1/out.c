// Выводит содержимое файла
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Проверка аргументов
    if (argc < 2)
    {
        exit(EXIT_FAILURE);
    }

    // Открытие файла
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        exit(EXIT_FAILURE);
    }

    // Открытие файла
    // Выделение памяти под строку размеров с файл
    off_t off = lseek(fd, 0, SEEK_END);
    char *buf = (char*)malloc(off * sizeof(char));

    // Чтение файла
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, off);
    printf("%s", buf);

    free (buf);
    exit(EXIT_SUCCESS);
}
