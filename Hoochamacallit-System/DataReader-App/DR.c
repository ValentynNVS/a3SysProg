#include "DR.h"
#include "errno.h"
int main(void)
{
    /* Create message queue if it doesn't already exist */
    int messageQueueID;
    key_t uniqueToken = getUniqueToken("/home", kMessageQueueSeed);
    if ((messageQueueID = messageQueueExists(uniqueToken)) == -1)
    {
        messageQueueID = createMessageQueue(uniqueToken);
    }
    else
    {
        msgctl(messageQueueID, IPC_RMID, 0);
        messageQueueID = createMessageQueue(uniqueToken);
    }

    /* Create shared memory if it doesn't already exist */
    int sharedMemoryID;
    uniqueToken = getUniqueToken("/home/aalmoune1948/Desktop/A3/a3SysProg/Hoochamacallit-System/DataReader-App/cmake-build-debug/",kSharedMemorySeed);
    if ((sharedMemoryID = sharedMemoryExists(uniqueToken)) == -1)
    {
        sharedMemoryID = createSharedMemory(uniqueToken);
    }

    /* Allocate shared memory */
    MasterList *master;
 	master = (MasterList *)shmat(sharedMemoryID, NULL, 0); // this allows the dr to attach to the shared memory and begin producing data to be read
    master->messageQueueID = messageQueueID;
    master->numberOfMachines = 0;

    logEvent("DR started. Sleeping for 15 seconds before processing messages.");
    sleep(1);

    QueueMessage message;
    int sizeOfMessage = sizeof(QueueMessage) - sizeof(long);
    int bytesRead;

    /* Main loop; gets machine messages and handles them */
    while (1)
    {
        /* Receive a message from DC */
        bytesRead = msgrcv(messageQueueID, &message, sizeOfMessage, kServerRegularMessage, 0);
        if (bytesRead == -1)
        {
            perror("Error receiving message: ");
            break;
        }

        updateMasterList(master, &message);
        removeInactiveMachines(master);

        /* Quit program if there are no active machines remaining */
        if (master->numberOfMachines < 1)
        {
            break;
        }

        sleep(1); //change to 1.5 seconds
    }

    /* Free allocated memory */
    msgctl(messageQueueID, IPC_RMID, 0); // clean up the message queue
    shmdt(master); // detaching from shared memory
    shmctl(sharedMemoryID, IPC_RMID, 0); // remove the shared memory resource
    logEvent("All DCs have gone offline or terminated – DR TERMINATING");

    return 0;
}



// This function checks whether shared memory exists for the specified token (returns -1 if it doesn't)
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
        char* nowFormatted = ctime(&now);
        nowFormatted[strlen(nowFormatted) - 1] = '\0';
        fprintf(logFile, "[%s] : %s\n", nowFormatted, msg);
        fclose(logFile);
    }
}

// This function updates the master list
void updateMasterList(MasterList *master, QueueMessage *msg) {
  	time_t currentTime = time(NULL);
    char logEntry[256];

    /* Message is from an existing machine */
    for (int i = 0; i < master->numberOfMachines; i++)
    {
        if (master->machines[i].machineID == msg->machineID)
        {
            if (msg->statusCode != 6)
            {
                master->machines[i].lastTimeHeardFrom = currentTime;
                snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] updated in master list – MSG RECEIVED – Status %d (%s)", i + 1, msg->machineID, msg->statusCode, msg->statusDescription);
                logEvent(logEntry);
            }

            if (msg->statusCode == 6)
            {
                snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] has gone OFFLINE – removing from master list", i + 1, msg->machineID);
                logEvent(logEntry);
                removeMachine(master,  i);
            }
            return;
        }
    }

    /* Add new machine */
    if (master->numberOfMachines < kMaxNumberOfMachines) {
        master->machines[master->numberOfMachines].machineID = msg->machineID;
        master->machines[master->numberOfMachines].lastTimeHeardFrom = currentTime;
        master->numberOfMachines++;
        char logEntry[256];
        snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] added to master list – NEW DC – Status 0 (Everything is OKAY)", master->numberOfMachines, msg->machineID);
        logEvent(logEntry);
    }
}

//This function removes inactive machines from the master list
void removeInactiveMachines(MasterList *master) {
  	time_t now = time(NULL);

    for (int i = 0; i < master->numberOfMachines; i++) {
        if (difftime(now, master->machines[i].lastTimeHeardFrom) > 35) {  // 35 sec timeout
            char logEntry[256];
            snprintf(logEntry, sizeof(logEntry), "DC-%02d [%d] removed from master list – NON-RESPONSIVE", i + 1, master->machines[i].machineID);
            logEvent(logEntry);
            removeMachine(master,  i);
        }
    }
}

// This function removes the specified machine from the master list
void removeMachine(MasterList* master, int machineIndex)
{
    if (machineIndex < 0 || machineIndex >= master->numberOfMachines)
    {
        perror("Invalid machine index: ");
        return;
    }

    /* Update master list */
    int i;
    for (i = machineIndex; i < master->numberOfMachines - 1; i++)
    {
        master->machines[i] = master->machines[i + 1];
    }

    /* The machine is the last one in the array (special case)*/
    if (i == master->numberOfMachines - 1)
    {
        MachineInfo emptyMachine = {};
        master->machines[i] = emptyMachine;
    }

    master->numberOfMachines--;
}