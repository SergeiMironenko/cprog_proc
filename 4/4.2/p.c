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

pid_t pid1, pid2;
FILE *fp;
int stop = 0;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};


void handler(int signo)
{
    switch(signo)
    {
        case SIGUSR1:
            stop = !stop;
            break;
        case SIGUSR2:
            stop = !stop;
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Введите количество случайных чисел первым аргументом.\n");
        exit(EXIT_FAILURE);
    }

    // Создание пустого файла
    char *filename = "out.txt";
    fp = fopen(filename, "w");
    fclose(fp);

    // Обработка сигналов
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    // Инициализация каналов
    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание семафора
    key_t key = ftok(".", 1);
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};

    pid1 = getpid();
    int cnt = 0;

    switch(pid2 = fork())
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            close(pipefd[0]);
            srand(time(NULL));
            int a;

            for (int i = 0; i < atoi(argv[1]); i++)
            {
                semop(semid, &lock, 1);

                // Чтение из файла
                fp = fopen(filename, "r");
                fseek(fp, 0, SEEK_END);
                int fsize = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                char *res = (char*)malloc((fsize + 1) * sizeof(char));
                fread(res, sizeof(char), fsize, fp);
                fclose(fp);
                if (fsize == 0) printf("file is empty\n");
                else printf("%s", res);
                free(res);

                // Генерация числа и отправка в канал
                a = rand() % 1000;
                write(pipefd[1], &a, sizeof(int));

                semop(semid, unlock, 2);
            }

            exit(EXIT_SUCCESS);
        default: ;
            close(pipefd[1]);
            int b;
            
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                // Чтение из канала
                read(pipefd[0], &b, sizeof(int));
                printf("number from PARENT = %d\n", b);

                semop(semid, &lock, 1);

                // Запись в файл
                fp = fopen(filename, "a");
                fprintf(fp, "from FILE: %d\n", b);
                fclose(fp);

                semop(semid, unlock, 2);
            }

            int status;
            wait(&status);
    }

    exit(EXIT_SUCCESS);
}
