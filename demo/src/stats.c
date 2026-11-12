/* Demo 1 - a segment is memory shared by two processes.
 *   ./stats_demo          the child writes its row, the parent reads it at once
 *   ./stats_demo write    the parent writes through its read-only pointer:
 *                         the kernel stops it (Segmentation fault) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"

struct row {
    pid_t pid;
    int queries;
    char state[12];
};

int main(int argc, char *argv[]) {
    // A private segment of one row, filled with zeros by the kernel
    int shmid = shmget(IPC_PRIVATE, sizeof(struct row), S_IRUSR | S_IWUSR);
    if (shmid == -1)
        errExit("shmget");
    printf("segment %d created: ipcs -m shows it (%zu bytes)\n", shmid, sizeof(struct row));

    // Attached twice: read and write, and read only
    struct row *rw = (struct row *)shmat(shmid, NULL, 0);
    const struct row *ro = (const struct row *)shmat(shmid, NULL, SHM_RDONLY);
    if (rw == (void *)-1 || ro == (void *)-1)
        errExit("shmat");
    printf("attached at %p (rw) and %p (ro): row = {%d, %d, \"%s\"}\n", (void *)rw, (const void *)ro, (int)ro->pid, ro->queries, ro->state);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == -1)
        errExit("fork");
    if (pid == 0) {
        // The child inherits the attachments: same segment, same pointers
        rw->pid = getpid();
        strcpy(rw->state, "running");
        for (int q = 1; q <= 3; q++) {
            rw->queries = q;
            printf("  child %d: queries = %d\n", getpid(), q);
            fflush(stdout);
            sleep(1);
        }
        strcpy(rw->state, "idle");
        exit(0);
    }
    for (int t = 0; t < 4; t++) {
        sleep(1);
        printf("parent reads: row = {%d, %d, \"%s\"}\n", (int)ro->pid, ro->queries, ro->state);
        fflush(stdout);
    }
    wait(NULL);

    if (argc > 1) {
        printf("parent writes through the read-only pointer...\n");
        fflush(stdout);
        ((struct row *)ro)->queries = 0;   // SIGSEGV: the mapping is read only
    }

    if (shmdt(rw) == -1 || shmdt(ro) == -1)
        errExit("shmdt");
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        errExit("shmctl IPC_RMID");
    printf("detached and removed\n");
    return 0;
}
