#include "query.h"

#include <stdio.h>

void printQuery(const struct query *q) {
    printf("executing [priority %ld] from PID %d: %s\n", q->mtype, (int)q->client, q->sql);
    fflush(stdout);
}
