#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main()
{
    int fd;
    char buf[80];
    char *fifofile = "fifofile";

    mkfifo(fifofile, 0666);

    // Создание и инициализация семафора
    key_t key = ftok("Makefile", 666);
    int semid = semget(key, 1, 0666);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};

    fd = open(fifofile, O_RDONLY);

    while (1)
    {
        if (semop(semid, &lock, 1) == -1)
        {
            perror("semop:lock");
            exit(EXIT_SUCCESS);
        }

        read(fd, buf, 80);
        printf("Read from fifofile: %s\n", buf);

        if (semop(semid, unlock, 2) == -1)
        {
            perror("semop:unlock");
            exit(EXIT_SUCCESS);
        }

        sleep(1);
    }

    close(fd);

    semctl(semid, 0, IPC_RMID);

    return 0;
}
