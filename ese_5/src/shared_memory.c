#include "shared_memory.h"

#include <sys/shm.h>
#include <sys/stat.h>

#include "errExit.h"

int alloc_shared_memory(key_t shmKey, size_t size) {
    // Get, or create, the segment using 'shmget' with the flags
    // IPC_CREAT | S_IRUSR | S_IWUSR (use 'errExit' if it fails)
    int shmid = -1;


    return shmid;
}

void *get_shared_memory(int shmid, int shmflg) {
    // Attach the segment using 'shmat', letting the kernel choose the
    // address (NULL); on error shmat returns (void *)-1 (use 'errExit')
    void *ptr_sh = NULL;


    return ptr_sh;
}

void free_shared_memory(void *ptr_sh) {
    // Detach the segment using 'shmdt' (use 'errExit' if it fails)

}

void remove_shared_memory(int shmid) {
    // Mark the segment for removal using 'shmctl' and IPC_RMID
    // (use 'errExit' if it fails)

}
