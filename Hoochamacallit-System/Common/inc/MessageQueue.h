//
// This file contains the necessary libraries, constants, prototypes, and structures that allow the client (DC) and server (DR) to
// establish a connection on a message queue.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

// Include statements
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>


// Constants
#define  kGeneralStringLength 100
#define  kMessageQueueSeed 'A'
#define  FAILURE 1

// Function prototypes
key_t getUniqueToken(int seed);
int messageQueueExists(key_t uniqueToken);
int createMessageQueue(key_t uniqueToken);

// Structures
typedef struct QueueMessage
{
    long type; // Required by LINUX
    pid_t machineID;
    int statusCode;
    char statusDescription[kGeneralStringLength];
} QueueMessage;


#endif //MESSAGEQUEUE_H
