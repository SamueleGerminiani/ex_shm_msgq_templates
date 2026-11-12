#ifndef _PAGE_HH
#define _PAGE_HH

#include <sys/types.h>

// A page of the table "accounts", as stored on disk (given)
#define PAGE_SIZE 1024
#define ROWS_PER_PAGE 8

// The request a backend leaves in the request segment
struct request {
    int pageNo;        // the page wanted
    int replyShmid;    // the segment where the buffer manager puts it
    pid_t backend;     // who asks
};

// The two semaphores of the buffer manager
#define REQUEST 0      // a request is in the request segment
#define READY 1        // the page is in the reply segment

// loadPage "reads" page pageNo from disk into buf (PAGE_SIZE bytes)
void loadPage(int pageNo, char *buf);

#endif
