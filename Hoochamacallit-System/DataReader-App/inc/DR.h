// This file contains the necessary libraries, constants, prototypes, and structures that the server (DR) needs
// to function. (i.e. receive messages from machines, keep track of active machines, log events, etc.)

#ifndef DR_H
#define DR_H

// Include statements
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "../../Common/inc/MessageQueue.h"

// Constants
#define  kMaxNumberOfMachines 10
#define  kSharedMemorySeed 16535
#define kServerRegularMessage 1
#define LOG_FILE "/tmp/dataMonitor.log"

// Structures
typedef struct MachineInfo
{
    pid_t machineID;
    time_t lastTimeHeardFrom;
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
void removeMachine(MasterList* master, int machineIndex);

#endif //DR_H
