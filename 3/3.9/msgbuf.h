#ifndef MSGBUF_H
#define MSGBUF_H

#define MTEXT_LEN 80

typedef struct msgbuf
{
    long mtype;
    char mtext[MTEXT_LEN];
} msgbuf;

#endif
