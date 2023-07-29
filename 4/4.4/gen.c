#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <semaphore.h>

int main()
{
    int fd;
    char buf[80];
    char *fifofile = "fifofile";

    srand(time(0));

    unlink(fifofile);
    mkfifo(fifofile, 0666);

    // Создание семафора
    sem_unlink("sem");
    sem_t *sem = sem_open("sem", O_CREAT, 0666, 3);

    while (1)
    {
        sem_wait(sem);

        int r = rand() % 100;
        snprintf(buf, 80, "%d", r);
        printf("Generated: %d\n", r);

        fd = open(fifofile, O_WRONLY);
        write(fd, buf, 80);
        close(fd);

        sem_post(sem);

        sleep(1);
    }

    return 0;
}
