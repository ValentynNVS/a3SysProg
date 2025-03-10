
#include "DR.h"


int main(void)
{
    key_t uniqueToken = getMessageQueueId();




    return 0;
}


// This function calculates the unique token used to create the message queue
key_t getUniqueToken(void) //**NOTE: should be in common
{
    key_t uniqueToken = ftok (".", kUniqueTokenSeed);
    if (uniqueToken == -1)
    {
        perror("Error creating key: ");
        exit(FAILURE);
    }

    return uniqueToken;
}
