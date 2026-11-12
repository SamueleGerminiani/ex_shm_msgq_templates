#include "page.h"

#include <stdio.h>
#include <string.h>

// The disk: page P holds the accounts P*8 ... P*8+7 (given)
void loadPage(int pageNo, char *buf) {
    char line[128];
    buf[0] = '\0';
    for (int r = 0; r < ROWS_PER_PAGE; r++) {
        int account = pageNo * ROWS_PER_PAGE + r;
        snprintf(line, sizeof(line), "page %d row %d: account %05d balance %6d.%02d\n",
                 pageNo, r, account, (account * 7919) % 10000, (account * 31) % 100);
        strncat(buf, line, PAGE_SIZE - strlen(buf) - 1);
    }
}
