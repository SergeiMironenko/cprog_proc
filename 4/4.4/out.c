#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <semaphore.h>

int main()
{
    int fd;
    char buf[80];
    char *fifofile = "fifofile";

    mkfifo(fifofile, 0666);

    // Создание и инициализация семафора
    sem_t *sem = sem_open("sem", O_CREAT, 0666, 1);

    fd = open(fifofile, O_RDONLY | O_NONBLOCK);

    while (1)
    {
        sem_wait(sem);

        read(fd, buf, 80);
        printf("Read from fifofile: %s\n", buf);

        sem_post(sem);

        sleep(1);
    }

    close(fd);

    return 0;
}
