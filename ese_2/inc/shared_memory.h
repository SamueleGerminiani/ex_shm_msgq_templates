#ifndef _SHARED_MEMORY_HH
#define _SHARED_MEMORY_HH

#include <stdlib.h>
#include <sys/types.h>

/* The four operations on a shared memory segment, each one a system call
 * plus its error check. Complete them in shared_memory.c. */

// alloc_shared_memory creates, or opens if it exists, a shared memory
// segment of size bytes with key shmKey (IPC_PRIVATE for a new private
// segment). It returns the shmid; on error it terminates the process.
int alloc_shared_memory(key_t shmKey, size_t size);

// get_shared_memory attaches the segment shmid to the address space of the
// calling process with the flags shmflg (0: read and write, SHM_RDONLY:
// read only). It returns the address; on error it terminates the process.
void *get_shared_memory(int shmid, int shmflg);

// free_shared_memory detaches the segment attached at ptr_sh.
// On error it terminates the process.
void free_shared_memory(void *ptr_sh);

// remove_shared_memory marks the segment shmid for removal: it disappears
// when the last process detaches it. On error it terminates the process.
void remove_shared_memory(int shmid);

#endif
