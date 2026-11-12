#ifndef _REPORT_HH
#define _REPORT_HH

#include <sys/types.h>

/* Release report code: you do not need to read or modify this file :)
 *
 * report_served is called by the postmaster for every query it executes.
 * report_check reveals code #3 when the postmaster, interrupted by SIGINT,
 * has served at least two queries sent by other processes and the queue
 * is empty. Call it before removing the queue.
 * The code is bound to the current user and machine.
 */
void report_served(pid_t client);
void report_check(int msqid, int interrupted);

#endif
