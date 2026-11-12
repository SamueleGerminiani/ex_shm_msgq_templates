#ifndef _REPORT_HH
#define _REPORT_HH

/* Release report code: you do not need to read or modify this file :)
 *
 * report_check reveals code #5 when the result segment holds every row
 * and was created by another process. Call it before detaching.
 * The code is bound to the current user and machine.
 */
void report_check(int shmid, int rows, const char *result);

#endif
