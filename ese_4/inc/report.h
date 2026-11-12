#ifndef _REPORT_HH
#define _REPORT_HH

/* Release report code: you do not need to read or modify this file :)
 *
 * report_served is called for every query executed, with its priority;
 * report_idle every time the queue was found empty.
 * report_check reveals code #4 when the postmaster idled, then served the
 * queries by priority. Call it before removing the queue.
 * The code is bound to the current user and machine.
 */
void report_served(long type);
void report_idle(void);
void report_check(int msqid, int expected);

#endif
