// This file contains the necessary libraries, constants, prototypes, and structures that the server (DR) needs
// to function. (i.e. receive messages from machines, keep track of active machines, log key events, etc.)
//
#ifndef DR_H
#define DR_H

// Include statements
#include <sys/ipc.h> //NOTE: Is this needed??
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include "../Common/inc/MessageQueue.h"

// Constants
#define  kMaxNumberOfMachines 10
#define  kSharedMemorySeed 16535
#define LOG_FILE "/tmp/dataMonitor.log"


// Structures
typedef struct MachineInfo
{
    pid_t machineID;
    int lastTimeHeardFrom;
} MachineInfo;

typedef struct MasterList
{
    int messageQueueID;
    int numberOfMachines;
    MachineInfo machines[kMaxNumberOfMachines];
} MasterList;

// Function prototypes
int sharedMemoryExists(key_t uniqueToken);
int createSharedMemory(key_t uniqueToken);
void logEvent(const char *msg);
void updateMasterList(MasterList *master, QueueMessage *msg);
void removeInactiveMachines(MasterList *master);

#endif //DR_H
