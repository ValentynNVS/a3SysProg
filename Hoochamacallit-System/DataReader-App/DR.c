
#include "DR.h"


int main(void)
{
    key_t uniqueToken = getMessageQueueId(kUniqueTokenSeed); //Create a unique token from a value agreed upon with the client i.e. 'A'

    int messageQueueID;
    if ((messageQueueID = messageQueueExists(uniqueToken)) == -1)
    {
        messageQueueID = createMessageQueue(uniqueToken);
    }



    return 0;
}


// This function calculates the unique token used to create the message queue
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
    if (messageQueueID == -1)
    {
        return -1;
    }
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