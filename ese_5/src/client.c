#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "errExit.h"
#include "report.h"
#include "result.h"
#include "shared_memory.h"

int main(int argc, char *argv[]) {
    // The queue: key from 'ftok' with "." and 'n', created with 'msgget'
    // and the flags IPC_CREAT | S_IRUSR | S_IWUSR (use 'errExit')
    key_t key = -1;
    int msqid = -1;


    printf("client %d: waiting for a result on queue %d\n", getpid(), msqid);
    fflush(stdout);

    // Wait for the notification using 'msgrcv': size NOTIFICATION_SIZE,
    // type 1, flags 0 (use 'errExit' if it fails)
    struct notification n;

    printf("client: %d rows from backend %d in segment %d\n", n.rows, (int)n.backend, n.shmid);

    // Attach the result segment in read only mode (SHM_RDONLY) with
    // get_shared_memory
    const char *result = NULL;

    // Print the first three rows and the last one (given)
    for (int i = 0; i < n.rows && i < 3; i++)
        printf("  %s\n", result + (size_t)i * ROW_LEN);
    if (n.rows > 4)
        printf("  ... %d more rows ...\n", n.rows - 4);
    if (n.rows > 3)
        printf("  %s\n", result + (size_t)(n.rows - 1) * ROW_LEN);

    // Release report (given): reveals code #5 when the result is complete
    report_check(n.shmid, n.rows, result);

    // Detach the segment with free_shared_memory and remove it with
    // remove_shared_memory: the reader cleans up. Then remove the queue
    // using 'msgctl' and IPC_RMID (use 'errExit' if it fails)



    printf("client: result read, segment and queue removed\n");

    return 0;
}
