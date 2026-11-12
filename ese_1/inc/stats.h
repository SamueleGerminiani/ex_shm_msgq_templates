#ifndef _STATS_HH
#define _STATS_HH

#include <sys/types.h>

// The statistics segment of the engine (given)
#define MAX_BACKENDS 8
#define QUERIES 5           // queries executed by each backend
#define PAGES_PER_QUERY 20  // pages scanned by a query
#define ROWS_PER_PAGE 100   // rows in a page

struct backendRow {
    pid_t pid;
    int queries;        // queries completed
    long rows;          // rows scanned
    char state[12];     // "running" or "idle"
};

struct stats {
    int backends;                        // rows in use
    long rowsScanned;                    // rows scanned by all backends
    struct backendRow row[MAX_BACKENDS];
};

// Prints the table (given)
void printStats(const struct stats *s);

#endif
