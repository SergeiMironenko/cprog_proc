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
#include <sys/mman.h>

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

    sem_t *sem = sem_open("sem", O_CREAT, 0666, 0);

    int shmid = shm_open("shm", O_CREAT | O_RDWR, 0666);
    ftruncate(shmid, 7 * sizeof(int));
    int *v = (int *)mmap(0, 7 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

    while (cont)
    {
        switch(pid = fork())
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                // Ожидание генерации чисел
                sem_wait(sem);

                // Поиск максимума и минимума
                int min = v[1], max = v[1];
                for (int i = 0; i < v[0]; i++)
                {
                    if (v[i + 1] < min) min = v[i + 1];
                    else if (v[i + 1] > max) max = v[i + 1];
                }

                // Вывод и отправка чисел
                printf("(child): max = %d, min = %d\n", max, min);
                v[0] = max;
                v[1] = min;

                shmdt(v);
                exit(EXIT_SUCCESS);
            default: ;
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

                // Ожидания поиска максимума и минимума
                sem_post(sem);
                wait(&status);

                // Вывод результата
                printf("(parent): max = %d, min = %d\n", v[0], v[1]);

        }

        sleep(1);
    }

    munmap(v, 7 * sizeof(int));
    shm_unlink("shm");

    sem_unlink("sem");
    sem_close(sem);
    exit(EXIT_SUCCESS);
}
