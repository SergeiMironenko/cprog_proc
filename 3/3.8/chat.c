#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

int main(int argc, char *argv[])
{
    // Проверка аргументов
    if (argc < 3)
    {
        printf("Программа принимает на вход два числа: ID того, кто отправляет, ID, того, кому отправляют.\n");
        exit(EXIT_FAILURE);
    }

    printf("Вы пользователь %d.\n", atoi(argv[1]));
    msgbuf msg;



    // Создание очереди на получение сообщений
    key_t key_rcv = ftok("README.md", atoi(argv[1]));
    int msqid_rcv = msgget(key_rcv, 0600 | IPC_CREAT);

    if (msqid_rcv == -1)
    {
        perror("msgget (rcv)");
        exit(EXIT_FAILURE);
    }

    // Проверка сообщений
    while (1)
    {
        int rcv_status = msgrcv(msqid_rcv, (void *) &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);
        
        if (rcv_status == -1)
        {
            printf("Сообщений не найдено.\n");
            break;
        }
        else
        {
            printf("(От пользователя %ld): %s", msg.mtype, msg.mtext);
        }
    }



    // Создание очереди на отправку сообщений
    key_t key_snd = ftok("README.md", atoi(argv[2]));
    int msqid_snd = msgget(key_snd, 0600 | IPC_CREAT);

    if (msqid_snd == -1)
    {
        perror("msgget (rcv)");
        exit(EXIT_FAILURE);
    }

    // Отправка сообщений
    while(1)
    {
        char buf[1000];
        printf("Введите сообщение (пользователю %d):\n> ", atoi(argv[2]));
        fgets(buf, 1000, stdin);

        msg.mtype = atoi(argv[1]);
        snprintf(msg.mtext, 1000, "%s", buf);
        int snd_status = msgsnd(msqid_snd, (void *)&msg, sizeof(msg.mtext), 0);

        if (snd_status == -1)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Сообщение добавлено в очередь.\n");
    }

    exit(EXIT_SUCCESS);
}
