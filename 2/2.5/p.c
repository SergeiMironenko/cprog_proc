#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

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
        exit(EXIT_FAILURE);
    }

    struct dirent *dir_ent;
    struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
    char path[100];
    while (dir_ent = readdir(dir))
    {
        // Составление пути к файлу
        path[0] = 0;
        strcat(path, argv[1]);
        strcat(path, dir_ent->d_name);
        printf("[path = %s]   ", path);

        // Проверка файла
        int stat_rv = stat(path, buf);
        if (stat_rv == 0)
        {
            char *type = (char*)malloc(sizeof(char));
            if (S_ISREG(buf->st_mode)) type = "regular file";
            else if (S_ISDIR(buf->st_mode)) type = "directory";
            else if (S_ISLNK(buf->st_mode)) type = "symbolic link";
            else if (S_ISSOCK(buf->st_mode)) type = "socket";
            else if (S_ISCHR(buf->st_mode)) type = "character device";
            else if (S_ISBLK(buf->st_mode)) type = "block device";
            else if (S_ISFIFO(buf->st_mode)) type = "FIFO channel";
            
            printf("%s   (%s)\n", dir_ent->d_name, type);
        }
        else
        {
            printf("errno = %d (%s)\n", errno, strerror(errno));
        }
    }
    
    free(buf);
    exit(EXIT_SUCCESS);
}
