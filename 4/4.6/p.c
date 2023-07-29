#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
// #include <signal.h>
#include <wait.h>
#include <sys/file.h>
// #include <sys/sem.h>
#include <semaphore.h>

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
        printf("1 аргумент - кол-во случайных чисел, 2 аргумент - количество процессов.\n");
        exit(EXIT_FAILURE);
    }

    // Создание пустого файла
    char *filename = "out.txt";
    FILE *fp = fopen(filename, "w");
    fclose(fp);

    // Инициализация каналов
    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание семафора
    unlink("sem_mutex");
    unlink("sem_push");
    unlink("sem_pop");
    sem_t *sem_mutex = sem_open("sem_mutex", O_CREAT, 0666, 1);
    sem_t *sem_push = sem_open("sem_push", O_CREAT, 0666, atoi(argv[2]));
    sem_t *sem_pop = sem_open("sem_pop", O_CREAT, 0666, 0);

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
                    sem_wait(sem_push);
                    sem_post(sem_pop);

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

                    sem_wait(sem_pop);
                    sem_post(sem_push);
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

        sem_wait(sem_push);
        sem_post(sem_pop);

        // Запись в файл
        fp = fopen(filename, "a");
        fprintf(fp, "from FILE: %d\n", b);
        fclose(fp);

        sem_wait(sem_pop);
        sem_post(sem_push);
    }

    int status;
    wait(&status);

    exit(EXIT_SUCCESS);
}
