#ifndef MSGBUF_H
#define MSGBUF_H

typedef struct msgbuf
{
    long mtype;
    char mtext[80];
} msgbuf;

#endif
