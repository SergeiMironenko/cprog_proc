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

pid_t pid1, pid2;
FILE *fp;

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

    // Инициализация каналов
    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание семафора
    sem_unlink("sem");
    sem_t *sem = sem_open("sem", O_CREAT, 0666, 1);

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
                sem_wait(sem);

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

                sem_post(sem);
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

                sem_wait(sem);

                // Запись в файл
                fp = fopen(filename, "a");
                fprintf(fp, "from FILE: %d\n", b);
                fclose(fp);

                sem_post(sem);
            }

            int status;
            wait(&status);
    }

    exit(EXIT_SUCCESS);
}
