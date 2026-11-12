#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "errExit.h"
#include "page.h"
#include "semaphore.h"
#include "shared_memory.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s <requests>\n", argv[0]);
        return 1;
    }

    // Convert argv[1] to an integer using 'atoi': the requests to serve
    int requests = 0;
    if (requests <= 0) {
        printf("The number of requests must be greater than 0!\n");
        return 1;
    }

    // The request segment: key from 'ftok' with "." and 'r', created with
    // alloc_shared_memory (sizeof(struct request) bytes) and attached in
    // read and write mode with get_shared_memory
    key_t shmKey = -1;
    int shmid = -1;
    struct request *req = NULL;

    // The semaphore set: key from 'ftok' with "." and 's', two semaphores
    // (REQUEST and READY) created with 'semget' (IPC_CREAT | S_IRUSR |
    // S_IWUSR) and both initialized to 0 with SETALL
    // (use 'errExit' if a call fails)
    key_t semKey = -1;
    int semid = -1;
    unsigned short initial[2] = {0 /*change this*/};
    union semun arg;
    arg.array = initial;


    printf("buffer manager %d: request segment %d, semaphores %d, serving %d request(s)\n", getpid(), shmid, semid, requests);
    fflush(stdout);

    for (int n = 1; n <= requests; n++) {
        // Wait for a request: semOp with -1 on REQUEST


        printf("request %d: page %d for backend %d, reply in segment %d\n", n, req->pageNo, (int)req->backend, req->replyShmid);
        fflush(stdout);

        // Attach the reply segment of the backend (req->replyShmid) in read
        // and write mode with get_shared_memory
        char *reply = NULL;

        // Read the page from disk straight into the reply segment (given)
        loadPage(req->pageNo, reply);

        // Detach the reply segment with free_shared_memory, then tell the
        // backend that the page is ready: semOp with +1 on READY


    }

    // Detach and remove the request segment (free_shared_memory,
    // remove_shared_memory), remove the semaphore set ('semctl', IPC_RMID)


    printf("buffer manager: done, request segment and semaphores removed\n");

    return 0;
}
