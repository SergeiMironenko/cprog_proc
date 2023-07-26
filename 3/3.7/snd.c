#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

int main(int argc, char *argv[])
{
    key_t key = ftok("README.md", 13);
    int msqid = msgget(key, 0600 | IPC_CREAT);
    
    if (msqid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    int msg_num = 5;
    for (int i = 0; i < msg_num; i++)
    {
        msgbuf msg;

        // Пред пред последнее сообщение будет иметь тип 255
        if (i == msg_num - 2) msg.mtype = 255;
        else msg.mtype = 1;

        snprintf(msg.mtext, 80, "sent number: %d", i);
        int snd_status = msgsnd(msqid, (void *)&msg, sizeof(msg.mtext), 0);

        if (snd_status == -1)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    printf("В очередь добавлено %d сообщений.\n", msg_num);
    exit(EXIT_SUCCESS);
}
