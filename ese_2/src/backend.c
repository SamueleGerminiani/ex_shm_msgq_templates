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
#include "report.h"
#include "semaphore.h"
#include "shared_memory.h"

int main(int argc, char *argv[]) {
    // Check command line input arguments using 'argc'
    if (argc != 2) {
        printf("Usage: %s <page>\n", argv[0]);
        return 1;
    }

    // Convert argv[1] to an integer using 'atoi': the page wanted
    int pageNo = 0;
    if (pageNo <= 0) {
        printf("The page number must be greater than 0!\n");
        return 1;
    }

    // The reply segment: a private segment (IPC_PRIVATE) of PAGE_SIZE bytes
    // created with alloc_shared_memory. Its identifier travels in the
    // request: an identifier is valid in every process, like a key
    int replyShmid = -1;

    // The request segment and the semaphores of the buffer manager: the
    // same keys ('ftok' with "." and 'r', "." and 's'). Open the segment
    // with alloc_shared_memory and the set with 'semget' (2 semaphores,
    // flags 0: it fails if the buffer manager is not running)
    key_t shmKey = -1;
    int shmid = -1;
    key_t semKey = -1;
    int semid = -1;


    // Attach the request segment in read and write mode and fill the
    // request: the page, the reply segment, the PID of this process
    struct request *req = NULL;




    printf("backend %d: asking for page %d, reply in segment %d\n", getpid(), pageNo, replyShmid);
    fflush(stdout);

    // Announce the request (semOp with +1 on REQUEST), then wait for the
    // page (semOp with -1 on READY)



    // Attach the reply segment in read only mode (SHM_RDONLY) and print
    // the page
    const char *page = NULL;


    // Release report (given): reveals code #2 when the page is the right one
    report_check(replyShmid, pageNo, page);

    // Detach both segments with free_shared_memory and remove the reply
    // segment with remove_shared_memory




    return 0;
}
