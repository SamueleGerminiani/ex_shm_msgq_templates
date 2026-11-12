#ifndef _REPORT_HH
#define _REPORT_HH

#include "stats.h"

/* Release report code: you do not need to read or modify this file :)
 *
 * report_check reveals code #1 when the statistics are exact and the
 * postmaster holds its two attachments. Call it before detaching.
 * The code is bound to the current user and machine.
 */
void report_check(int shmid, const struct stats *s, int backends);

#endif
