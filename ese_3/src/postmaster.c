#include <errno.h>
#include <signal.h>
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

// Set by the handler of SIGINT
volatile int shutdownRequested = 0;

// The handler of SIGINT: print "shutdown requested" and set the flag
void sigHandler(int sig) {

}

int main(int argc, char *argv[]) {
    // Install sigHandler for SIGINT using 'signal'
    // (use 'errExit' if it returns SIG_ERR)


    // The key of the queue: 'ftok' with "." and 'q' (use 'errExit')
    key_t key = -1;


    // Create the queue using 'msgget' with the key and the flags
    // IPC_CREAT | S_IRUSR | S_IWUSR (use 'errExit' if it fails)
    int msqid = -1;


    printf("postmaster %d: queue %d (key 0x%x) ready, Ctrl+C to shut down\n", getpid(), msqid, key);
    fflush(stdout);

    int served = 0;
    while (!shutdownRequested) {
        // Receive the next query using 'msgrcv': the size is QUERY_SIZE,
        // the type 0 (the first message of the queue), the flags 0.
        // If it fails with errno EINTR the call was interrupted by SIGINT:
        // continue (the while condition ends the loop); any other error
        // is fatal (errExit)
        struct query q;
        if (1 /*change this*/) {



        }

        // Execute the query (given)
        printQuery(&q);
        report_served(q.client);
        served++;
        sleep(1);
    }

    printf("postmaster: %d quer%s executed\n", served, served == 1 ? "y" : "ies");

    // Release report (given): reveals code #3 after at least two queries
    report_check(msqid, shutdownRequested);

    // Remove the queue using 'msgctl' and IPC_RMID (use 'errExit' if it fails)

    printf("postmaster: queue removed\n");

    return 0;
}
