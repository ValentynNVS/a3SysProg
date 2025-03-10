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

// Constants
#define  kGeneralStringLength  100
#define  kUniqueTokenSeed      'A' // used by the server and the client to get the same unique token
#define  FAILURE		       1

// Function prototypes
key_t getMessageQueueId(void);


/* The structure of a queue message */
typedef struct queueMessage //**NOTE: should be in common
{
    long type; // Required by LINUX
    pid_t machineID;
    int statusCode;
    char statusDescription[kGeneralStringLength];
} queueMessage;



#endif //DR_H
