// Вывод содержимого указанного каталога

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(EXIT_SUCCESS);
    }

    DIR *dir = opendir(argv[1]);
    if (dir == NULL)
    {
        printf("Директории %s не существует.\n", argv[1]);
        exit(EXIT_SUCCESS);
    }

    struct dirent *dir_ent;
    while (dir_ent = readdir(dir))
    {
        printf("%s\n", dir_ent->d_name);
    }

    exit(EXIT_SUCCESS);
}
