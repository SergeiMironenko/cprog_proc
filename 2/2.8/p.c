#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Введите количество случайных чисел первым аргументом.\n");
        exit(EXIT_FAILURE);
    }

    int pipefd1[2];  // Дочерний процесс -> родитель
    int pipefd2[2];  // Родитель -> дочерний процесс
    if(pipe(pipefd1) || pipe(pipefd2))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            // Закрытие неиспользуемых дескрипторов
            close(pipefd1[0]);
            close(pipefd2[1]);

            srand(time(NULL));
            int random_int, doubled_int_from_parent;

            for (int i = 0; i < atoi(argv[1]); i++)
            {
                random_int = rand() % 1000;  // 0 <= x < 1000
                write(pipefd1[1], &random_int, sizeof(int));  // Запись сгенерированного числа
                read(pipefd2[0], &doubled_int_from_parent, sizeof(int));  // Чтение удвоенного числа

                printf("from parent: %d\n", doubled_int_from_parent);
            }
            exit(EXIT_SUCCESS);
        default:
            // Закрытие неиспользуемых дескрипторов
            close(pipefd1[1]);
            close(pipefd2[0]);

            int int_from_child, doubled_int_to_child;
            FILE *fp = fopen("out", "w");

            for (int i = 0; i < atoi(argv[1]); i++)
            {
                read(pipefd1[0], &int_from_child, sizeof(int));  // Чтение сгенерированного числа
                doubled_int_to_child = 2 * int_from_child;
                write(pipefd2[1], &doubled_int_to_child, sizeof(int));  // Запись удвоенного числа

                fprintf(fp, "from pipe: %d\n", int_from_child);
                printf("from pipe = %d\n", int_from_child);
            }
            fclose(fp);
    }
    exit(EXIT_SUCCESS);
}
