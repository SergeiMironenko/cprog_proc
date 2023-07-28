#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sem.h>

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

    srand(time(0));

    unlink(fifofile);
    mkfifo(fifofile, 0666);

    // Создание семафора
    key_t key = ftok("Makefile", 666);
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализация
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};

    while (1)
    {
        if (semop(semid, &lock, 1) == -1)
        {
            perror("semop:lock");
            exit(EXIT_FAILURE);
        }

        int r = rand() % 100;
        snprintf(buf, 80, "%d", r);
        printf("Generated: %d\n", r);

        fd = open(fifofile, O_WRONLY | O_NONBLOCK);
        write(fd, buf, 80);
        close(fd);

        sleep(1);

        if (semop(semid, unlock, 2) == -1)
        {
            perror("semop:unlock");
            exit(EXIT_FAILURE);
        }
    }

    semctl(semid, 0, IPC_RMID);

    return 0;
}
