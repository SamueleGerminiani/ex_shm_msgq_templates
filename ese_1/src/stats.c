#include "stats.h"

#include <stdio.h>

void printStats(const struct stats *s) {
    printf("--- backend     queries   rows      state\n");
    for (int i = 0; i < s->backends; i++)
        printf("    %-11d %-9d %-9ld %s\n", (int)s->row[i].pid, s->row[i].queries, s->row[i].rows, s->row[i].state);
    printf("    rows scanned by all backends: %ld\n", s->rowsScanned);
    fflush(stdout);
}
