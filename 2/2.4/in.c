// Дописывает в файл строку.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define STR_LEN 30  // Максимальная длина строки

int main(int argc, char *argv[])
{
    // Проверка аргументов
    if (argc < 2)
    {
        exit(EXIT_FAILURE);
    }

    // Открытие / создание файла
    int fd = open(argv[1], O_WRONLY | O_APPEND, S_IRWXU);
    if (fd == -1)
    {
        char c = 0, x = 0;
        while (c != 'y' && c != 'n')
        {
            printf("Файла %s не существует. Хотите создать данный файл ? (y/n)", argv[1]);
            scanf("%c%c", &c, &x);
            while (x != '\n') x = getchar();  // Очистка stdin
            switch(c)
            {
                case 'y':
                    fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
                    break;
                case 'n':
                    exit(EXIT_SUCCESS);
            }
        }
    }

    // Считывание строки
    char buf[STR_LEN];
    printf("Введите строку: ");
    fgets(buf, STR_LEN, stdin);

    // Запись в файл и закрытие файла
    write(fd, buf, strlen(buf));
    close(fd);

    exit(EXIT_SUCCESS);
}
