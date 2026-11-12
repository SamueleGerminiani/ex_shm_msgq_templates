#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "errExit.h"
#include "query.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s \"<sql>\"\n", argv[0]);
        return 1;
    }

    // The key of the queue: 'ftok' with "." and 'q', exactly as the
    // postmaster (use 'errExit' if it fails)
    key_t key = -1;


    // Open the existing queue using 'msgget' with the key and the flags
    // S_IRUSR | S_IWUSR (no IPC_CREAT): it fails with "No such file or
    // directory" if the postmaster is not running (use 'errExit')
    int msqid = -1;


    // Fill the message: type 1, the PID of this process, the text of
    // argv[1] copied with 'strncpy' into sql (at most sizeof(q.sql) - 1
    // characters, then the terminator)
    struct query q;





    // Send it using 'msgsnd' with the size QUERY_SIZE and flags 0
    // (use 'errExit' if it fails)


    printf("client %d: query sent to the postmaster\n", getpid());
    return 0;
}
