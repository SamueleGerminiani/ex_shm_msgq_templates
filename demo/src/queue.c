/* Demo 3 - a message queue: typed messages, received in the order you ask.
 * The parent sends three queries with types 3, 1, 2. The child receives
 * the first one with type 0 (whatever comes first), then the others with
 * type -3 (the lowest type first). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"

struct query {
    long mtype;          // 1 interactive, 2 batch, 3 maintenance
    char sql[64];
};
#define QUERY_SIZE (sizeof(struct query) - sizeof(long))

int main(int argc, char *argv[]) {
    int msqid = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);
    if (msqid == -1)
        errExit("msgget");
    printf("queue %d created: ipcs -q shows it\n", msqid);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == -1)
        errExit("fork");
    if (pid == 0) {
        struct query q;
        sleep(1);   // let the parent fill the queue first
        if (msgrcv(msqid, &q, QUERY_SIZE, 0, 0) == -1)   // type 0: the first in the queue
            errExit("msgrcv");
        printf("  child, type 0 : [%ld] %s\n", q.mtype, q.sql);
        for (int i = 0; i < 2; i++) {
            if (msgrcv(msqid, &q, QUERY_SIZE, -3, 0) == -1)   // type -3: lowest type <= 3
                errExit("msgrcv");
            printf("  child, type -3: [%ld] %s\n", q.mtype, q.sql);
        }
        exit(0);
    }

    struct query queries[3] = {{3, "VACUUM accounts"}, {1, "SELECT * FROM accounts WHERE id = 7"}, {2, "COPY accounts TO '/backup/accounts.csv'"}};
    for (int i = 0; i < 3; i++) {
        if (msgsnd(msqid, &queries[i], QUERY_SIZE, 0) == -1)
            errExit("msgsnd");
        printf("parent sent      : [%ld] %s\n", queries[i].mtype, queries[i].sql);
    }
    fflush(stdout);
    wait(NULL);
    if (msgctl(msqid, IPC_RMID, NULL) == -1)
        errExit("msgctl IPC_RMID");
    printf("queue removed\n");
    return 0;
}
