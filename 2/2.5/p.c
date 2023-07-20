#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        exit(EXIT_FAILURE);
    }
    struct dirent *dir_ent;
    struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
    while (dir_ent = readdir(dir))
    {
        stat(dir_ent->d_name, buf);

        char type = '-';
        if (S_ISREG(buf->st_mode)) type = 'f';
        else if (S_ISDIR(buf->st_mode)) type = 'd';
        
        printf("(%c) %s\n", type, dir_ent->d_name);
    }
    free(buf);


    exit(EXIT_SUCCESS);
}
