#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    char *fifofile = "fifofile";

    mkfifo(fifofile, 0666);

    char buf[80];
    while (1)
    {
        fd = open(fifofile, O_RDONLY);
        read(fd, buf, 80);
        close(fd);

        printf("Read from fifofile: %s\n", buf);
    }
    return 0;
}
