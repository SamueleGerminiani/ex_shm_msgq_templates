#ifndef _QUERY_HH
#define _QUERY_HH

#include <stdlib.h>
#include <sys/types.h>

// A query sent to the postmaster through the message queue (given)
struct query {
    long mtype;        // 1: interactive, 2: batch, 3: maintenance
    pid_t client;      // who sent it
    char sql[200];     // the text of the query
};

// The size of the body of the message: everything but mtype
#define QUERY_SIZE (sizeof(struct query) - sizeof(long))

// Prints a query (given)
void printQuery(const struct query *q);

#endif
