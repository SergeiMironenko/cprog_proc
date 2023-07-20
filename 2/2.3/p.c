#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main()
{
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        exit(EXIT_FAILURE);
    }
    struct dirent *dir_ent;
    while (dir_ent = readdir(dir))
    {
        printf("%s\n", dir_ent->d_name);
    }

    exit(EXIT_SUCCESS);
}
