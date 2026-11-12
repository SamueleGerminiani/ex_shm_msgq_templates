/* Demo 2 - shared memory does not synchronise anything.
 *   ./race_demo        two children add 1 to a shared counter 100000 times
 *                      each: the total is less than 200000 (lost updates)
 *   ./race_demo lock   each increment is protected by a semaphore: exact */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"
#include "semaphore.h"

#define INCREMENTS 100000

int main(int argc, char *argv[]) {
    int lock = argc > 1;

    int shmid = shmget(IPC_PRIVATE, sizeof(long), S_IRUSR | S_IWUSR);
    if (shmid == -1)
        errExit("shmget");
    long *counter = (long *)shmat(shmid, NULL, 0);
    if (counter == (void *)-1)
        errExit("shmat");

    int semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
    if (semid == -1)
        errExit("semget");
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
        errExit("semctl SETVAL");

    printf("two backends, %d increments each, %s\n", INCREMENTS, lock ? "with the latch" : "no latch");
    fflush(stdout);
    for (int b = 0; b < 2; b++) {
        pid_t pid = fork();
        if (pid == -1)
            errExit("fork");
        if (pid == 0) {
            for (int i = 0; i < INCREMENTS; i++) {
                if (lock)
                    semOp(semid, 0, -1);
                (*counter)++;      // read, add, write: three steps
                if (lock)
                    semOp(semid, 0, 1);
            }
            exit(0);
        }
    }
    while (wait(NULL) != -1)
        ;
    printf("counter = %ld (expected %d)\n", *counter, 2 * INCREMENTS);

    if (shmdt(counter) == -1)
        errExit("shmdt");
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        errExit("shmctl IPC_RMID");
    if (semctl(semid, 0, IPC_RMID, 0) == -1)
        errExit("semctl IPC_RMID");
    return 0;
}
