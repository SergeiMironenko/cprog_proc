#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    int fd;
    char buf[80];
    char *fifofile = "fifofile";

    srand(time(0));

    mkfifo(fifofile, 0666);

    while (1)
    {
        int r = rand() % 100;
        snprintf(buf, 80, "%d", r);
        printf("Generated: %d\n", r);

        fd = open(fifofile, O_WRONLY);
        write(fd, buf, 80);
        close(fd);

        sleep(1);
    }
    return 0;
}
