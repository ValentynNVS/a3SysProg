//
// This file contains the necessary functions that allow the client (DC) and server (DR) to
// establish a connection on a message queue.
//
#include "../inc/MessageQueue.h"


// This function calculates a unique token based on a seed
key_t getUniqueToken(int seed)
{
    key_t uniqueToken = ftok (".", seed);
    if (uniqueToken == -1)
    {
        perror("Error creating unique token: ");
        exit(FAILURE);
    }
    return uniqueToken;
}

// This function checks whether a message queue exists for the specified token
int messageQueueExists(key_t uniqueToken)
{
    int messageQueueID = msgget(uniqueToken, 0);
    return messageQueueID;
}

// This function creates a message queue with the specified token
int createMessageQueue(key_t uniqueToken)
{
    int messageQueueID = msgget (uniqueToken, IPC_CREAT | 0660);
    if (messageQueueID == -1)
    {
        perror("Error creating message queue: ");
        exit(FAILURE);
    }
    return messageQueueID;
}
