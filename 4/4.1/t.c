#include <stdio.h>
#include <stdlib.h>
// #include <errno.h>
// #include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
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
    key_t key;
    pid_t pid;
    int semid;

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};

    // Создание семафора
    key = ftok("Makefile", 666);
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафора
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    // Переключение семафора
    semop(semid, &lock, 1);

    semop(semid, unlock, 2);
    // semop(semid, &unlock, 1);

    // Чтение семафора
    union semun arg2;
    arg2.val = semctl(semid, 0, GETVAL);
    printf("GETVAL = %d\n", arg2.val);

    return 0;
}
