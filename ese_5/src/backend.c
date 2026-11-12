#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "errExit.h"
#include "result.h"
#include "shared_memory.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s <rows>\n", argv[0]);
        return 1;
    }

    // Convert argv[1] to an integer using 'atoi': the rows of the result
    int rows = 0;
    if (rows <= 0) {
        printf("The number of rows must be greater than 0!\n");
        return 1;
    }

    // The result segment: a private segment (IPC_PRIVATE) of rows * ROW_LEN
    // bytes created with alloc_shared_memory and attached in read and
    // write mode with get_shared_memory
    int shmid = -1;
    char *result = NULL;

    // Execute the query: row i goes at result + i * ROW_LEN (given)
    for (int i = 0; i < rows; i++)
        makeRow(i, result + (size_t)i * ROW_LEN);
    printf("backend %d: %d rows (%zu bytes) in segment %d\n", getpid(), rows, (size_t)rows * ROW_LEN, shmid);

    // Detach the segment with free_shared_memory: the rows stay in it


    // The queue of the client: key from 'ftok' with "." and 'n', opened
    // with 'msgget' and the flags S_IRUSR | S_IWUSR (no IPC_CREAT: it
    // fails if the client is not waiting) (use 'errExit')
    key_t key = -1;
    int msqid = -1;


    // Notify the client: a message of type 1 with the segment, the rows
    // and the PID of this process, sent with 'msgsnd' (NOTIFICATION_SIZE,
    // flags 0) (use 'errExit' if it fails)
    struct notification n;






    // The segment is not removed here: with nobody attached, IPC_RMID would
    // destroy it at once, rows included. The client removes it after reading
    printf("backend %d: client notified, segment %d left for the client\n", getpid(), shmid);

    return 0;
}
