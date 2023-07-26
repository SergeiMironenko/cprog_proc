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

    while (1)
    {
        msgbuf msg;
        
        int rcv_status = msgrcv(msqid, (void *) &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);

        if (rcv_status == -1)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("text = %s, type = %ld\n", msg.mtext, msg.mtype);
        if (msg.mtype == 255)
        {
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
