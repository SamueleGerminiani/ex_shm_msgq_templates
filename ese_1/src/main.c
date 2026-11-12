#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errExit.h"
#include "report.h"
#include "semaphore.h"
#include "shared_memory.h"
#include "stats.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s <backends>\n", argv[0]);
        return 1;
    }

    // Convert argv[1] to an integer using 'atoi': the backend processes
    int backends = 0;
    if (backends <= 0 || backends > MAX_BACKENDS) {
        printf("The number of backends must be between 1 and %d!\n", MAX_BACKENDS);
        return 1;
    }

    // Create the statistics segment: a private segment (IPC_PRIVATE) of
    // sizeof(struct stats) bytes, with alloc_shared_memory
    int shmid = -1;

    // Attach it in read and write mode (flags 0) with get_shared_memory
    struct stats *stats = NULL;

    // A new segment is filled with zeros: only the number of rows is set
    stats->backends = backends;

    // Create a semaphore set with one semaphore (IPC_PRIVATE) and initialize
    // it to 1 with 'semctl' and SETVAL: the latch on the shared counter
    // (use 'errExit' if a call fails)
    int semid = -1;
    union semun arg;
    arg.val = 0 /*change this*/;


    printf("postmaster %d: statistics in segment %d, %d backend(s)\n", getpid(), shmid, backends);
    fflush(stdout);   // the buffer would be duplicated by fork

    // Create the backends using 'fork'. The children inherit the attached
    // segment: they use the same pointer, without calling shmat again
    for (int i = 0; i < backends; i++) {
        pid_t pid = -1;

        if (1 /*change this*/) {
            // fork failed: print a message using 'errExit'

        } else if (1 /*change this*/) {
            // Backend i: its row of the table
            struct backendRow *me = &stats->row[i];
            me->pid = getpid();

            for (int q = 1; q <= QUERIES; q++) {
                strcpy(me->state, "running");
                for (int page = 1; page <= PAGES_PER_QUERY; page++) {
                    // Scan a page: ROWS_PER_PAGE increments of the shared
                    // counter rowsScanned. Hold the latch for the whole
                    // page: semOp with -1 before, +1 after

                    for (int r = 0; r < ROWS_PER_PAGE; r++)
                        stats->rowsScanned++;


                    // The row of this backend is written by this backend only
                    me->rows += ROWS_PER_PAGE;
                }
                me->queries++;
                strcpy(me->state, "idle");
                sleep(1);
            }

        }
        // Postmaster: keep creating backends
    }

    // Postmaster: attach the segment a second time, in read only mode
    // (SHM_RDONLY), with get_shared_memory: the pointer of the monitor
    const struct stats *monitor = NULL;

    // Until every backend has terminated, print the table through the read
    // only pointer once a second. Use 'waitpid' with -1 and WNOHANG to
    // collect the terminated backends (use 'errExit' if it fails)
    int done = 0;
    while (done < backends) {
        pid_t pid = -1;






        sleep(1);
        printStats(monitor);
    }

    printf("every backend terminated: final statistics\n");
    printStats(monitor);

    // Release report (given): reveals code #1 when the statistics are exact
    report_check(shmid, monitor, backends);

    // Detach both pointers with free_shared_memory, then remove the segment
    // with remove_shared_memory and the semaphore set with 'semctl' and
    // IPC_RMID (use 'errExit' if it fails)





    return 0;
}
