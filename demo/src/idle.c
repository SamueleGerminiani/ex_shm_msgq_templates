/* Demo 4 - what a queue does when there is nothing to receive.
 *   1. msgrcv with IPC_NOWAIT on an empty queue: ENOMSG, at once
 *   2. msgctl IPC_STAT: how many messages are in the queue
 *   3. a blocked msgrcv ends when the queue is removed: EIDRM
 *   4. a message bigger than the limit: EINVAL (see MSGMAX) */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"

struct note {
    long mtype;
    char text[16384];
};

int main(int argc, char *argv[]) {
    int msqid = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);
    if (msqid == -1)
        errExit("msgget");
    struct note n;

    // 1. nothing to receive, do not wait
    if (msgrcv(msqid, &n, sizeof(n.text), 0, IPC_NOWAIT) == -1 && errno == ENOMSG)
        printf("1. msgrcv with IPC_NOWAIT: no message (ENOMSG), returned at once\n");

    // 2. the state of the queue
    struct msqid_ds ds;
    if (msgctl(msqid, IPC_STAT, &ds) == -1)
        errExit("msgctl IPC_STAT");
    printf("2. IPC_STAT: %ld message(s) in the queue, limit %ld bytes\n", (long)ds.msg_qnum, (long)ds.msg_qbytes);

    // 4. too big for one message
    n.mtype = 1;
    if (msgsnd(msqid, &n, sizeof(n.text), 0) == -1)
        printf("4. msgsnd of %zu bytes: %s (MSGMAX: a message is small; big data goes in shared memory)\n", sizeof(n.text), errno == EINVAL ? "Invalid argument" : "failed");

    // 3. a reader blocked on an empty queue, and the queue goes away
    fflush(stdout);
    pid_t pid = fork();
    if (pid == -1)
        errExit("fork");
    if (pid == 0) {
        printf("3. child: waiting for a message...\n");
        fflush(stdout);
        if (msgrcv(msqid, &n, sizeof(n.text), 0, 0) == -1)
            printf("3. child: msgrcv failed: %s\n", errno == EIDRM ? "Identifier removed (EIDRM)" : "another error");
        exit(0);
    }
    sleep(2);
    if (msgctl(msqid, IPC_RMID, NULL) == -1)
        errExit("msgctl IPC_RMID");
    printf("3. parent: queue removed while the child was waiting\n");
    wait(NULL);
    return 0;
}
