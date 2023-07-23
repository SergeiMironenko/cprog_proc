// Простой файловый менеджер
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

// Вывод содержимого директории
int dir_info(char *current_dir)
{
    // Проверка директории
    DIR *dir = opendir(current_dir);  // По умолчанию порядок файлов случайный
    if (dir == NULL)
    {
        printf("Директории %s не существует.\n", current_dir);
        return 1;
    }

    struct dirent *dir_ent;
    struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
    char *type = (char*)malloc(sizeof(char));
    char path[100];
    while (dir_ent = readdir(dir))
    {
        // Составление пути к файлу
        path[0] = 0;
        strcat(path, current_dir);
        strcat(path, dir_ent->d_name);

        // Проверка файла
        int stat_rv = lstat(path, buf);
        if (stat_rv == 0)
        {
            if (S_ISREG(buf->st_mode)) type = "regular file";
            else if (S_ISDIR(buf->st_mode)) type = "directory";
            else if (S_ISLNK(buf->st_mode)) type = "symbolic link";
            else if (S_ISSOCK(buf->st_mode)) type = "socket";
            else if (S_ISCHR(buf->st_mode)) type = "character device";
            else if (S_ISBLK(buf->st_mode)) type = "block device";
            else if (S_ISFIFO(buf->st_mode)) type = "FIFO channel";
            
            printf("%-20s %s\n", type, dir_ent->d_name);
        }
        else
        {
            printf("errno = %d (%s)\n", errno, strerror(errno));
        }
    }
    
    free(buf);
    // free(type);
    return 0;
}

int main(int argc, char *argv[])
{
    char current_dir[100];  // Текущая директория
    char path[100];  // Вводимый путь (абсолютный или относительный)

    while (1)
    {
        // Вывод текущей директории (по умолчанию директория, из которой запускается программа)
        getcwd(current_dir, 100);
        if (strlen(current_dir) >0 && current_dir[strlen(current_dir) - 1] != '/')
        {
            strcat(current_dir, "/");  // Добавление "/" к пути файла
        }
        printf("Текущая директория: %s\n", current_dir);

        // Вывод содержимого директории
        dir_info(current_dir);

        // Пользовательский ввод
        printf("Введите директорию или exit: ");
        fgets(path, 100, stdin);
        path[strlen(path) - 1] = 0;  // Удалить символ конца строки
        system("clear");

        // Условие выхода из цикла
        if (strcmp(path, "exit") == 0) break;

        // Проверка пользовательских данных
        if (chdir(path))
        {
            printf("Невозможно перейти по пути: %s%s. ", current_dir, path);
            printf("Код ошибки: %d (%s).\n", errno, strerror(errno));
        }
    }
    exit(EXIT_SUCCESS);
}
