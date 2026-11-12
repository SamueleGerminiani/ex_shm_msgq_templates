#include "result.h"

#include <stdio.h>
#include <string.h>

void makeRow(int i, char *buf) {
    memset(buf, 0, ROW_LEN);
    snprintf(buf, ROW_LEN, "row %06d | balance %8d", i, (i * 7919) % 100000);
}
