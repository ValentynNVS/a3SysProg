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
    else
    {
        msgctl(messageQueueID, IPC_RMID, 0); // clean up the message queue
        messageQueueID = createMessageQueue(uniqueToken);
    }

    /* Create shared memory if it doesn't already exist */
    int sharedMemoryID;
    uniqueToken = getUniqueToken(kSharedMemorySeed);
    if ((sharedMemoryID = sharedMemoryExists(uniqueToken)) == -1)
    {
        sharedMemoryID = createSharedMemory(uniqueToken);
    }

    MasterList *master;
 	master = (MasterList *)shmat(sharedMemoryID, NULL, 0); // this allows the dr to attach to the shared memory and begin producing data to be read
    master->messageQueueID = messageQueueID;
    master->numberOfMachines = 0;

    logEvent("DR started. Sleeping for 15 seconds before processing messages.");
    sleep(1);

    QueueMessage message;
    int sizeOfMessage = sizeof(QueueMessage) - sizeof(long);
    int bytesRead;

    while (1)
    {
        /* Receive a message from DC */
        bytesRead = msgrcv(messageQueueID, &message, sizeOfMessage, kServerRegularMessage, 0);
        if (bytesRead == -1)
        {
            perror("Error receiving message: ");
            break;
        }


        // call update list function

        // call removeInactiveMachine function

        if (master->numberOfMachines < 1)
        {
            break;
        }

        sleep(1); //change to 1.5 seconds


    }


    shmdt(master); // detaching from shared memory
    shmctl(sharedMemoryID, IPC_RMID, 0); // remove the shared memory resource
    logEvent("Message queue and shared memory removed. DR shutting down.");
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

// This function logs the events happening into a log file
void logEvent(const char *msg) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile) {
        time_t now = time(NULL);
        fprintf(logFile, "[%s] : %s\n", ctime(&now), msg);
        fclose(logFile);
    }
}

// This function updates the masterlist
void updateMasterList(MasterList *master, QueueMessage *msg) {
  	time_t currentTime = time(NULL);
    int found = 0;

    for (int i = 0; i < master->numberOfMachines; i++)
    {
        if (master->machines[i].machineID == msg->machineID)
        {
            // make sure to only log non- 06 messages.
            master->machines[i].lastTimeHeardFrom = currentTime;
            char logEntry[256];
            snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] updated in master list – MSG RECEIVED – Status %d (%s)", i + 1, msg->machineID, msg->statusCode, msg->statusDescription);
            logEvent(logEntry);

            if (msg->statusCode == 6)
            {  // Status 6 = DC offline
                snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] has gone OFFLINE – removing from master list", i + 1, msg->machineID);
                logEvent(logEntry);
                removeInactiveMachines(master); //move call to main (add call to removeMachine instead)
            }
            return;
        }
    }


    if (master->numberOfMachines < kMaxNumberOfMachines) {
        master->machines[master->numberOfMachines].machineID = msg->machineID;
        master->machines[master->numberOfMachines].lastTimeHeardFrom = currentTime;
        master->numberOfMachines++;
        char logEntry[256];
        snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] added to master list – NEW DC – Status 0 (Everything is OKAY)", master->numberOfMachines, msg->machineID);
        logEvent(logEntry);
        //remove machine if code is 06
    }
}

//This function removes inactive machines from the masterlist
void removeInactiveMachines(MasterList *master) {
  	time_t now = time(NULL);

    for (int i = 0; i < master->numberOfMachines; i++) {
        if (difftime(now, master->machines[i].lastTimeHeardFrom) > 35) {  // 35 sec timeout
            char logEntry[256];
            snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] removed from master list – NON-RESPONSIVE", i + 1, master->machines[i].machineID);
            logEvent(logEntry);
            //remove machine
        }
    }
}

