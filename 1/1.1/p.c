#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    uid_t euid = geteuid();
    gid_t gid = getgid();
    pid_t sid = getsid(pid);

    printf("pid = %d\n", pid);
    printf("ppid = %d\n", ppid);
    printf("uid = %d\n", uid);
    printf("euid = %d\n", euid);
    printf("gid = %d\n", gid);
    printf("sid = %d\n", sid);

    return 0;
}
