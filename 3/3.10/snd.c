#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "erproc.h"

#define BUF_LEN 80

int main(int argc, char *argv[])
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = BUF_LEN;
    attr.mq_curmsgs = 0;
    mqd_t mqd = Mq_open("/mqueue", O_CREAT | O_WRONLY | O_NONBLOCK, 0600, &attr);
    int msg_num = 3;
    char buf[BUF_LEN];

    for (int i = 0; i < msg_num; i++)
    {
        snprintf(buf, BUF_LEN, "sent number: %d", i);
        Mq_send(mqd, buf, BUF_LEN, 1);
    }

    printf("В очередь добавлено %d сообщений.\n", msg_num);
    exit(EXIT_SUCCESS);
}
