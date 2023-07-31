#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "msgbuf.h"
#define MAX_USERS 10  // Максимальное количество пользователей

int main(int argc, char *argv[])
{
    // Проверка аргументов
    if (argc < 2)
    {
        printf("Введите аргумент - ID сервера / пользователя.\n");
        exit(EXIT_FAILURE);
    }

    // Проверка количества пользователей
    if (atoi(argv[1]) < 0 || atoi(argv[1]) > MAX_USERS)
    {
        printf("Неверное значение: 0 - сервер. 1 - %d - пользователи.\n", MAX_USERS);
        exit(EXIT_FAILURE);
    }

    msgbuf msg;
    pid_t pid;

    // Сервер
    if (atoi(argv[1]) == 0)
    {
        printf("Запущен сервер.\n");

        // Создание очереди на получение сообщений
        key_t key = ftok("README.md", 13);
        int msqid = msgget(key, 0600 | IPC_CREAT);

        // Проверка сообщений
        while (1)
        {
            int rcv_status = msgrcv(msqid, (void *) &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);

            if (rcv_status != -1)
            {
                // Если сообщение не обработано
                if (msg.mtype <= MAX_USERS)
                {
                    printf("Получено сообщение: mtype = %ld, mtext = %s", msg.mtype, msg.mtext);

                    // Добавление id к сообщению
                    char new_message[MTEXT_LEN];
                    snprintf(new_message, MTEXT_LEN, "(id = %ld): ", msg.mtype);
                    strcat(new_message, msg.mtext);
                    strcpy(msg.mtext, new_message);

                    // Отправка сообщений
                    for (int i = 0; i < MAX_USERS; i++)
                    {
                        msg.mtype = i + 1 + MAX_USERS;  // 1 <= type <= 10 - необработаны, 11 <= type <= 20 - необработаны
                        int snd_status = msgsnd(msqid, (void *)&msg, sizeof(msg.mtext), 0);

                        if (snd_status == -1)
                        {
                            perror("msgsnd");
                            exit(EXIT_FAILURE);
                        }
                    }

                    printf("Сообщения добавлены в очередь.\n");
                }
                else
                {
                    // Отправка сообщения без изменений
                    int snd_status = msgsnd(msqid, (void *)&msg, sizeof(msg.mtext), 0);

                    if (snd_status == -1)
                    {
                        perror("msgsnd");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }


    // Клиент
    else
    {
        printf("Запущен клиент\n");

        // Подключение к  очереди 
        key_t key = ftok("README.md", 13);
        int msqid = msgget(key, 0600);

        if (msqid == -1)
        {
            perror("msgget (rcv)");
            exit(EXIT_FAILURE);
        }

        char opt = 0, c = '\n';

        while (opt != 'q')
        {
            printf("1 - Проверить сообщения\n2 - Отправить сообщение\nq - Выйти\n");
            scanf("%c%c", &opt, &c);
            while (c != '\n') c = getchar();

            switch(opt)
            {
                case '1': ;
                    // Проверка сообщений
                    int rcv_num = 0;  // Количество считанных сообщений
                    while (1)
                    {
                        int rcv_type = atoi(argv[1]) + MAX_USERS;
                        int rcv_status = msgrcv(msqid, (void *) &msg, sizeof(msg.mtext), rcv_type, IPC_NOWAIT);
                        
                        if (rcv_status == -1)
                        {
                            if (rcv_num == 0) printf("Сообщений не найдено.\n");
                            break;
                        }
                        else
                        {
                            rcv_num++;
                            printf("%s\n", msg.mtext);
                        }
                    }
                    break;
                case '2': ;
                    // Отправка сообщений
                    char buf[MTEXT_LEN];
                    printf("Введите сообщение: ");
                    fgets(buf, MTEXT_LEN, stdin);

                    msg.mtype = atoi(argv[1]);
                    snprintf(msg.mtext, MTEXT_LEN, "%s", buf);
                    int snd_status = msgsnd(msqid, (void *)&msg, sizeof(msg.mtext), 0);

                    if (snd_status == -1)
                    {
                        perror("msgsnd");
                        exit(EXIT_FAILURE);
                    }

                    printf("Сообщение добавлено в очередь.\n");
                    break;
                case 'q':
                    exit(EXIT_SUCCESS);
            }
        }
    }

    exit(EXIT_SUCCESS);
}
