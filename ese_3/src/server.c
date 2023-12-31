#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#include "errExit.h"
#include "order.h"

// the message queue identifier
int msqid = -1;

void sigIntHandler(int sig) {
  // do we have a valid message queue identifier?
  if (msqid > 0) {
    //...
  }

  // terminate the server process
  exit(0);
}

int main(int argc, char *argv[]) {
  // check command line input arguments
  if (argc != 2) {
    printf("Usage: %s message_queue_key\n", argv[0]);
    exit(1);
  }

  // read the message queue key defined by user
  int msgKey = atoi(argv[1]);
  if (msgKey <= 0) {
    printf("The message queue key must be greater than zero!\n");
    exit(1);
  }

  // set of signals (N.B. it is not initialized!)
  sigset_t mySet;
  // blocking all signals but SIGINT
  // ...

  // set the function sigHandler as handler for the signal SIGINT
  // ...

  printf("<Server> Making MSG queue...\n");
  // get the message queue, or create a new one if it does not exist
  msqid =  // ...

  // check functionality
  printf("<Server> sleep...\n");

  struct order order;
  // endless loop
  while (1) {
    errno = 0;
    size_t mSize = sizeof(struct order) - sizeof(long);
    // read a message from the message queue.
    // type is set equal to -2. Thus, first all messages with type 1 are read.
    // When no message with type 1 is in the queue, then messages with type
    // 2 are read from the queue.
    // IPC_NOWAIT is sest as flag for msgrcv. Thus, if no message is in the
    // queue the system call returns immediately with errno equals to ENOMSG
    // ...

    // print the order on standard output
    printOrder(&order);
  }

  return 0;
}
