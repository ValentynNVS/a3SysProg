#ifndef DX_H
#define DX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

// I made this in base of the requirements
#define MAX_DC_ROLES 10 //here "the DR Master list stores up to 10 DC processes"
#define SHM_KEY_SEED 16535 // here "The DC will try to get the shared-memory ID value using" the given key
#define MAX_RETRIES 1 //here "DX application will sleep for 10 seconds and try again" "This re-try will continue until 100 retries"
#define MIN_WAIT 10 // here "1. Sleep for a random amount of time 
#define MAX_WAIT 30	//(between 10 and 30 seconds)"

typedef struct {
    pid_t dcProcessID; // here I store the process ID of a DC so DX can kill it if it is necessary
    time_t lastTimeHeardFrom; // and here I store the last time DR received a message from DC
} DCInfo;

typedef struct {
    int msgQueueID; // this line stores the message queue ID for DC-DR communication
    int numberOfDCs; // this other line tracks the number of active DC processes
    DCInfo dc[MAX_DC_ROLES]; // here I store the details about each active DC
} MasterList;

#endif