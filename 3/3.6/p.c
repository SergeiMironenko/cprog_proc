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

pid_t pid1, pid2;
FILE *fp;
int stop = 0;

void handler(int signo)
{
    switch(signo)
    {
        case SIGUSR1:
            // printf("stop changing to %d...\n", !stop);
            stop = !stop;
            // printf("stop changed %d...\n", stop);
            break;
        case SIGUSR2:
            // printf("stop changing to %d...\n", !stop);
            stop = !stop;
            // printf("stop changed %d...\n", stop);
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

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    // Обработка сигналов
    // struct sigaction sa;
    // sa.sa_handler = handler;
    // sigaction(SIGUSR1, &sa, NULL);
    // sigaction(SIGUSR2, &sa, NULL);

    int pipefd[2];
    if(pipe(pipefd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

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
                // Ожидание записи в родительском процессе
                // !!! Иногда программа не выходит из цикла ???
                // printf("before while (stop = %d)...\n", stop);
                while (stop) ;

                // Чтение из файла
                // printf("reading (stop = %d)...\n", stop);
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
                // printf("generating (stop = %d)...\n", stop);
                a = rand() % 1000;
                write(pipefd[1], &a, sizeof(int));
                // printf("pipe wrote (stop = %d)\n", stop);
            }

            exit(EXIT_SUCCESS);
        default: ;
            close(pipefd[1]);
            int b;
            
            for (int i = 0; i < atoi(argv[1]); i++)
            {
                // Чтение из канала
                // printf("pipe waiting...\n");
                read(pipefd[0], &b, sizeof(int));
                printf("Number from parent = %d\n", b);

                // Блокировка дочернего процесса
                // printf("blocking...\n");
                kill(pid2, SIGUSR1);
                // printf("blocked\n");

                // Запись в файл
                // printf("file writing...\n");
                fp = fopen(filename, "a");
                fprintf(fp, "Number in file: %d\n", b);
                fclose(fp);

                // Разблокировка дочернего процесса
                // printf("unblocking...\n");
                kill(pid2, SIGUSR2);
                // printf("unblocked\n");
            }

            int status;
            wait(&status);
    }

    exit(EXIT_SUCCESS);
}
