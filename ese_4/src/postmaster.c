#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "errExit.h"
#include "query.h"
#include "report.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s <queries>\n", argv[0]);
        return 1;
    }

    // Convert argv[1] to an integer using 'atoi': the queries to execute
    int queries = 0;
    if (queries <= 0) {
        printf("The number of queries must be greater than 0!\n");
        return 1;
    }

    // The key of the queue: 'ftok' with "." and 'q' (use 'errExit')
    key_t key = -1;


    // Create the queue using 'msgget' with the key and the flags
    // IPC_CREAT | S_IRUSR | S_IWUSR (use 'errExit' if it fails)
    int msqid = -1;


    printf("postmaster %d: queue %d ready, executing %d queries by priority\n", getpid(), msqid, queries);
    fflush(stdout);

    int served = 0;
    while (served < queries) {
        // Receive the most urgent query using 'msgrcv': size QUERY_SIZE,
        // type -3 (the lowest type not above 3), flags IPC_NOWAIT.
        // If it fails with errno ENOMSG the queue is empty: read the queue
        // length with 'msgctl' and IPC_STAT (field msg_qnum of a struct
        // msqid_ds), print "idle: no queries (queue length <n>)", call
        // report_idle (given), 'sleep' 2 seconds and try again.
        // Any other error is fatal (errExit)
        struct query q;
        if (1 /*change this*/) {











        }

        // Execute the query (given)
        printQuery(&q);
        report_served(q.mtype);
        served++;
        sleep(1);
    }

    // Release report (given): reveals code #4 when the three queries sent
    // during an idle period were executed by priority
    report_check(msqid, queries);

    // Remove the queue using 'msgctl' and IPC_RMID (use 'errExit' if it fails)

    printf("postmaster: queue removed\n");

    return 0;
}
