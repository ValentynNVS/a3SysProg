//
// Created by aalmoune1948 on 10/03/25.
//
#ifndef DR_H
#define DR_H

// Include statements
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <unistd.h>

// Constants
#define  kMaxNumberOfMachines 10
#define  kGeneralStringLength 100
#define  kMessageQueueSeed 'A' // used by the server and the client to get the same unique token
#define  kSharedMemorySeed 16535
#define  FAILURE 1

// Function prototypes
key_t getUniqueToken(int seed);
int messageQueueExists(key_t uniqueToken);
int createMessageQueue(key_t uniqueToken);
int sharedMemoryExists(key_t uniqueToken);
int createSharedMemory(key_t uniqueToken);

// Structures
typedef struct QueueMessage
{
    long type; // Required by LINUX
    pid_t machineID;
    int statusCode;
    char statusDescription[kGeneralStringLength];
} QueueMessage;

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


#endif //DR_H
