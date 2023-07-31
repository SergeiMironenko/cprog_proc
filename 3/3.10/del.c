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
    mqd_t mqd = Mq_open("/mqueue", O_CREAT | O_RDONLY | O_NONBLOCK, 0600, &attr);
    mq_unlink("/mqueue");
    mq_close(mqd);
    printf("Очередь удалена.\n");
    exit(EXIT_SUCCESS);
}
