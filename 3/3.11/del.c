#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
    // key_t key = ftok("README.md", 13);
    // int msqid = msgget(key, 0600);
    
    // if (msqid == -1)
    // {
    //     perror("msgget");
    //     exit(EXIT_FAILURE);
    // }

    // if (msgctl(msqid, IPC_RMID, NULL) == -1)
    // {
    //     perror("msgctl (remove queue)");
    //     exit(EXIT_FAILURE);
    // }

    printf("Очередь удалена.\n");
    exit(EXIT_SUCCESS);
}
