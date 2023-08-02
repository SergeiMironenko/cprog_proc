#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include "erproc.h"

#define MAX_USERS 2  // Максимальное количество пользователей
#define BUF_LEN 80

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

    pid_t pid;
    char buf[BUF_LEN];
    int prio;

    // Сервер
    if (atoi(argv[1]) == 0)
    {
        printf("Запущен сервер.\n");

        // Создание очереди на получение сообщений
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 5;
        attr.mq_msgsize = BUF_LEN;
        attr.mq_curmsgs = 0;
        mqd_t mqdes = Mq_open("/q", O_CREAT | O_RDWR, 0600, &attr);

        // Проверка сообщений
        while (1)
        {
            Mq_receive(mqdes, buf, BUF_LEN, &prio);
            // printf("received: %s\n", buf);

            // if (rcv_status != -1)
            // {
                // Если сообщение не обработано
                if (buf[0] == 1)
                {
                    printf("Получено сообщение: client_ud = %d, mtext = %s", buf[1], &buf[2]);
                    

                    // Отправка сообщений
                    for (int i = 0; i < MAX_USERS; i++)
                    {
                        // Добавление id к сообщению
                        char buf_copy[BUF_LEN];
                        strcpy(buf_copy, buf);
                        char tmpbuf[BUF_LEN];
                        snprintf(tmpbuf, BUF_LEN, "(id = %d): ", i + 1);
                        strcat(buf_copy, tmpbuf);
                        buf_copy[0] = 2;  // Сообщение обработано
                        Mq_send(mqdes, buf_copy, BUF_LEN, 1);
                    }

                    printf("Сообщения добавлены в очередь.\n");
                }
                else
                {
                    // Отправка сообщения без изменений
                    Mq_send(mqdes, buf, BUF_LEN, 1);
                }
            // }
        }
    }


    // Клиент
    else
    {
        printf("Запущен клиент\n");
        char client_id = atoi(argv[1]) % 256;

        // Подключение к  очереди 
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 5;
        attr.mq_msgsize = BUF_LEN;
        attr.mq_curmsgs = 0;
        mqd_t mqdes = Mq_open("/q", O_CREAT | O_RDWR, 0600, &attr);

        char opt = 0, c = '\n';

        while (opt != 'q')
        {
            printf("1 - Считать одно сообщение\n2 - Отправить сообщение\nq - Выйти\n");
            scanf("%c%c", &opt, &c);
            while (c != '\n') c = getchar();

            switch(opt)
            {
                case '1': ;
                    // Проверка сообщений
                    ssize_t n = Mq_receive(mqdes, buf, BUF_LEN, &prio);
                    if (n <= 0 || buf[1] != client_id)
                    {
                        printf("Сообщений не найдено.\n");
                    }
                    else
                    {
                        printf("%s (%d %d)\n", buf, buf[1], client_id);
                    }
                    break;
                case '2': ;
                    // Отправка сообщений
                    printf("Введите сообщение: ");
                    fgets(&buf[2], BUF_LEN, stdin);
                    buf[0] = 1;  // Сообщение не обработано
                    buf[1] = client_id;
                    Mq_send(mqdes, buf, BUF_LEN, 1);
                    printf("Сообщение добавлено в очередь (%d).\n", client_id);
                    break;
                case 'q':
                    exit(EXIT_SUCCESS);
            }
        }
    }

    exit(EXIT_SUCCESS);
}
