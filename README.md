# Shared memory and message queues: lab templates

A small database engine: a postmaster, its backend processes, a buffer
manager and the clients. Each exercise is one exchange between those
processes, written from a partially completed template: statistics in a
shared segment, pages served through two segments, queries sent as messages,
priorities, and a result set too big for a message.

```
ex_shm_msgq_templates/
├── demo/      <- Part 1: the programs shown during the lecture
├── ese_1/     <- Exercise 1: the statistics collector (shared memory, fork)
├── ese_2/     <- Exercise 2: the buffer manager (two programs, two segments)
├── ese_3/     <- Exercise 3: the query queue (two programs)
├── ese_4/     <- Exercise 4: priorities and an idle postmaster
├── ese_5/     <- Exercise 5: the result set (memory + queue)
└── report/    <- the release report (needs the 5 confirmation codes)
```

The exercises are independent: solve them in any order. Exercises 1, 2
and 5 have shared_memory.c to complete (the four operations on a segment)
besides the program sources; semaphore.c is given, completed in the
semaphores lab.

To compile:
> cd ese_1
> make

To run the executable
> ./shm_ese1 4

To remove object file(s) and executable(s)
> make clean

To list and remove the segments and queues left behind by a crashed run
> ipcs -m        ipcrm -m <shmid>
> ipcs -q        ipcrm -q <msqid>

To file the release report
> ./report/file_report.sh

It can be filed only with the 5 confirmation codes: exercise N reveals
code N when its result is the expected one.
