#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/sem.h>

FILE *fp;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("1 аргумент - кол-во случайных чисел, 2 аргумент - клдичество процессов.\n");
        exit(EXIT_FAILURE);
    }

    // Создание пустого файла
    char *filename = "out.txt";
    fp = fopen(filename, "w");
    fclose(fp);

    // Инициализация каналов
    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание семафора
    key_t key = ftok(".", 1);
    int semid = semget(key, 3, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    arg.val = atoi(argv[2]);
    semctl(semid, 1, SETVAL, arg);
    arg.val = 0;
    semctl(semid, 2, SETVAL, arg);

    // Операции семафоров
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};
    struct sembuf push[2] = {{1, -1, IPC_NOWAIT}, {2, 1, IPC_NOWAIT}};
    struct sembuf pop[2] = {{1, 1, IPC_NOWAIT}, {2, -1, IPC_NOWAIT}};

    pid_t pid;
    int cnt = 0;

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        switch(pid = fork())
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                close(pipefd[0]);
                
                int a;
                srand(getpid());

                for (int j = 0; j < atoi(argv[1]); j++)
                {
                    semop(semid, push, 2);
                    semop(semid, &lock, 1);

                    // Чтение из файла
                    fp = fopen(filename, "r");
                    fseek(fp, 0, SEEK_END);
                    int fsize = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    char *res = (char*)malloc((fsize + 1) * sizeof(char));
                    fread(res, sizeof(char), fsize, fp);
                    fclose(fp);
                    if (fsize == 0) printf("(child %d): file is empty\n", j);
                    else printf("(child %d): %s", j, res);
                    free(res);

                    // Генерация числа и отправка в канал
                    a = rand() % 1000;
                    write(pipefd[1], &a, sizeof(int));

                    semop(semid, unlock, 2);
                    semop(semid, pop, 2);
                }

                exit(EXIT_SUCCESS);
        }
    }
    
    // Родительский процесс
    close(pipefd[1]);
    int b;
    
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        // Чтение из канала
        read(pipefd[0], &b, sizeof(int));
        printf("number from PARENT = %d\n", b);

        semop(semid, push, 2);
        semop(semid, &lock, 1);

        // Запись в файл
        fp = fopen(filename, "a");
        fprintf(fp, "from FILE: %d\n", b);
        fclose(fp);

        semop(semid, unlock, 2);
        semop(semid, pop, 2);
    }

    int status;
    wait(&status);

    exit(EXIT_SUCCESS);
}
