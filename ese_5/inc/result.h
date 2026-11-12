#ifndef _RESULT_HH
#define _RESULT_HH

#include <stdlib.h>
#include <sys/types.h>

// A row of the result set: ROW_LEN characters, '\0' included (given)
#define ROW_LEN 32

// The notification sent through the queue: not the result, only where it is
struct notification {
    long mtype;        // always 1
    int shmid;         // the segment that holds the result
    int rows;          // rows in the segment, ROW_LEN bytes each
    pid_t backend;     // who produced it
};

#define NOTIFICATION_SIZE (sizeof(struct notification) - sizeof(long))

// makeRow writes row number i of the result into buf (ROW_LEN bytes)
void makeRow(int i, char *buf);

#endif
