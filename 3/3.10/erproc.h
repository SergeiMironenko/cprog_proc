#ifndef ERPROC_H
#define ERPROC_H

mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

void Mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

ssize_t Mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);

#endif
