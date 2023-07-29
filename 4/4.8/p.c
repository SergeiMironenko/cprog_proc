#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

int cont = 1;

void handler(int signo)
{
    cont = 0;
    printf("\n");
}

int main()
{
    srand(time(NULL));
    signal(SIGINT, handler);
    pid_t pid;
    int status;

    key_t key = ftok(".", 1);
    int shmid = shmget(key, 100, 0666 | IPC_CREAT);
    sem_t *sem = sem_open("sem", O_CREAT, 0666, 0);

    float *v = (float*)shmat(shmid, NULL, 0);

    while (cont)
    {
        // Максимум
        switch(pid = fork())
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                sem = sem_open("sem", O_CREAT, 0666, 0);

                // Ожидание генерации чисел
                sem_wait(sem);
                printf("(child %d) working...\n", getpid());

                // Поиск максимума
                float max = v[1];
                for (int i = 0; i < (int)v[0]; i++)
                {
                    if (v[i + 1] > max) max = v[i + 1];
                }

                // Вывод и отправка чисел
                v[(int)v[0] + 1] = max;

                exit(EXIT_SUCCESS);
        }

        // Минимум
        switch(pid = fork())
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                sem = sem_open("sem", O_CREAT, 0666, 0);

                // Ожидание генерации чисел
                sem_wait(sem);

                // Поиск минимума
                float min = v[1];
                for (int i = 0; i < (int)v[0]; i++)
                {
                    if (v[i + 1] < min) min = v[i + 1];
                }

                // Вывод и отправка чисел
                v[(int)v[0] + 2] = min;

                exit(EXIT_SUCCESS);
        }

        // Среднее
        switch(pid = fork())
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                sem = sem_open("sem", O_CREAT, 0666, 0);

                // Ожидание генерации чисел
                sem_wait(sem);

                // Поиск среднего
                float mid = 0;
                for (int i = 0; i < (int)v[0]; i++)
                {
                    mid += v[i + 1];
                }
                mid /= v[0];

                // Вывод и отправка чисел
                v[(int)v[0] + 3] = mid;

                exit(EXIT_SUCCESS);
        }

        // Генерация чисел: v[0] - кол-во чисел, далее следуют сами числа
        int imax = rand() % 5 + 2;
        v[0] = imax;
        printf("Сгенерированные числа (%d):", imax);
        for (int i = 0; i < imax; i++)
        {
            int r = rand() % 10;
            v[i + 1] = r;
            printf(" %d", r);
        }
        printf("\n");

        sem_post(sem);
        sem_post(sem);
        sem_post(sem);

        for (int i = 0; i < 3; i++)
        {
            pid = wait(&status);
            printf("child exited (%d)\n", pid);
        }

        // Вывод результата
        printf("(parent): max = %.2f, min = %.2f, mid = %.2f\n\n", v[(int)v[0] + 1], v[(int)v[0] + 2], v[(int)v[0] + 3]);
        sleep(1);
    }

    shmdt(v);
    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
