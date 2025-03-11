
#include "DR.h"


int main(void)
{
    /* Create message queue if it doesn't already exist */
    int messageQueueID;
    key_t uniqueToken = getUniqueToken(kMessageQueueSeed);
    if ((messageQueueID = messageQueueExists(uniqueToken)) == -1)
    {
        messageQueueID = createMessageQueue(uniqueToken);
    }

    /* Create shared memory if it doesn't already exist */
    int sharedMemoryID;
    uniqueToken = getUniqueToken(kSharedMemorySeed);
    if ((sharedMemoryID = sharedMemoryExists(uniqueToken)) == -1)
    {
        sharedMemoryID = createSharedMemory(uniqueToken);
    }

    sleep(15);


    
    return 0;
}


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

// This function checks whether shared memory exists for the specified token
int sharedMemoryExists(key_t uniqueToken)
{
    int sharedMemoryID = shmget(uniqueToken, sizeof (MasterList), 0);
    return sharedMemoryID;
}

// This function creates shared memory with the specified token
int createSharedMemory(key_t uniqueToken)
{
    int sharedMemoryID = shmget (uniqueToken, sizeof (MasterList), IPC_CREAT | 0660);
    if (sharedMemoryID == -1)
    {
        perror("Error creating shared memory: ");
        exit(FAILURE);
    }
    return sharedMemoryID;
}