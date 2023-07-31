#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include "erproc.h"

#define BUF_LEN 80

int main(int argc, char *argv[])
{
    // Проверка аргументов
    if (argc < 3)
    {
        printf("Программа принимает на вход два числа: ID того, кто отправляет, ID, того, кому отправляют.\n");
        exit(EXIT_FAILURE);
    }

    printf("Вы пользователь %d.\n", atoi(argv[1]));

    char buf[BUF_LEN];
    buf[0] = -1;
    int prio;



    // Очередль для отправки сообщений
    struct mq_attr attr_rcv;
    attr_rcv.mq_flags = 0;
    attr_rcv.mq_maxmsg = 5;
    attr_rcv.mq_msgsize = BUF_LEN;
    attr_rcv.mq_curmsgs = 0;
    mqd_t mqdes_rcv = Mq_open("/mq", O_CREAT | O_RDONLY | O_NONBLOCK, 0600, &attr_rcv);

    // Проверка сообщений
    while (1)
    {
        Mq_receive(mqdes_rcv, buf, BUF_LEN, &prio);
        if ((int)buf[0] == atoi(argv[1]))
        {
            printf("(От пользователя %d): %s", (int)buf[0], buf);
        }
    }



    // Создание очереди на отправку сообщений
    struct mq_attr attr_snd;
    attr_snd.mq_flags = 0;
    attr_snd.mq_maxmsg = 5;
    attr_snd.mq_msgsize = BUF_LEN;
    attr_snd.mq_curmsgs = 0;
    mqd_t mqdes_snd = Mq_open("/mq", O_CREAT | O_WRONLY | O_NONBLOCK, 0600, &attr_snd);

    // Отправка сообщений
    while(1)
    {
        printf("Введите сообщение (пользователю %d):\n> ", atoi(argv[2]));
        buf[0] = argv[2];
        char line[BUF_LEN];
        fgets(line, BUF_LEN, stdin);
        strcat(buf, line);

        Mq_send(mqdes_snd, buf, BUF_LEN, 1);
        printf("Сообщение добавлено в очередь.\n");
    }

    exit(EXIT_SUCCESS);
}
