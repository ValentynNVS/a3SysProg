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