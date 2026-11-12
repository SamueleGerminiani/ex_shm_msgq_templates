#ifndef _REPORT_HH
#define _REPORT_HH

/* Release report code: you do not need to read or modify this file :)
 *
 * report_check reveals code #2 when the reply segment holds the requested
 * page and only the backend is attached to it. Call it before detaching.
 * The code is bound to the current user and machine.
 */
void report_check(int replyShmid, int pageNo, const char *page);

#endif
