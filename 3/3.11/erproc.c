#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "erproc.h"
#include <string.h>
#include <errno.h>

mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr)
{
    mqd_t res = mq_open(name, oflag, mode, attr);
    if (res == (mqd_t) -1)
    {
        perror("mq_open failed");
        printf("errno: %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

void Mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
{
    int res = mq_send(mqdes, msg_ptr, msg_len, msg_prio);
    if (res == -1)
    {
        if (errno == EAGAIN)
        {
            printf("queue is full\n");
        }
        else
        {
            printf("mq_send failed");
            exit(EXIT_FAILURE);
        }
    }
}

ssize_t Mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio)
{
    ssize_t res = mq_receive(mqdes, msg_ptr, msg_len, msg_prio);
    if (res == -1)
    {
        if (errno == EAGAIN)
        {
            printf("queue is empty\n");
        }
        else
        {
            perror("mq_receive failed");
            exit(EXIT_FAILURE);
        }
    }
    return res;
}
