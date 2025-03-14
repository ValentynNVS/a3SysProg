#include "DX.h"


//Here What I want to achieve is to Record every DX action, Add timestamps to each event for analysis, Ensures logs persist 
//even after the DX exits and last but not least is that it mostly helps verify if is working without printing it to console (requirements)
void log_event(const char* message) {
    FILE* logFile = fopen("/tmp/dataCorruptor.log", "a");
    if (logFile) {
        time_t rawtime;
        struct tm* timeinfo;
        char timestamp[20];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        fprintf(logFile, "[%s] : %s\n", timestamp, message);
        fclose(logFile);
    }
}


//This is the main of my DX where I did the core logic of this corruptor, what this code does is that
//it attaches to shared memory, randomly chooses a destruction action and last but not least it executes them at intervals
int main() {
    // This first part of the code initializes the shared Memory Key creating a unique key for the shared memory
    key_t shmKey = ftok("/home/aalmoune1948/Desktop/A3/a3SysProg/Hoochamacallit-System/DataReader-App/cmake-build-debug/", SHM_KEY_SEED);
    int shmID, retries = 0;
    MasterList* sharedMemory;

    // In this while I try to attach to shared memory (this would be retried Up to 100 times)
    // if it doesnt exist then it sleeps for 10 seconds and retries
    while ((shmID = shmget(shmKey, sizeof(MasterList), 0)) == -1) {
        if (++retries > MAX_RETRIES) {
            log_event("DX failed to find shared memory after 100 retries, exiting.");
            return EXIT_FAILURE;
        }
        sleep(MIN_WAIT);
    }

    // here I attach to shared memory if it fails DX logs an error and exits.
    sharedMemory = (MasterList*)shmat(shmID, NULL, 0);
    if (sharedMemory == (void*)-1) {
        log_event("DX failed to attach to shared memory.");
        return EXIT_FAILURE;
    }

    srand(time(NULL));  // here seed generates a Random number ( this is needed for the random choices)


    //Here I decided to create a Infinite loop, because DX keeps running forever, randomly deciding when and how to attack
    while (1) {
        // here I put it to sleep between 10 and 30 seconds (which also prevents it to act to quickly)
        int waitTime = (rand() % (MAX_WAIT - MIN_WAIT + 1)) + MIN_WAIT;
        sleep(waitTime);

        // Check if message queue exists (if msgQ still exists)
        // but if the message queue is deleted, DX assumes DR & DC's have exited
        struct msqid_ds buffer;
        if (msgctl(sharedMemory->msgQueueID, IPC_STAT, &buffer) == -1)
        {
            log_event("DX detected that msgQ is gone assuming DR/DCs done");
            shmdt(sharedMemory);
            return EXIT_SUCCESS;
        }
        // if (msgget(sharedMemory->msgQueueID, 0) == -1) {
        //     log_event("DX detected that msgQ is gone assuming DR/DCs done");
        //     shmdt(sharedMemory);
        //     return EXIT_SUCCESS;
        // }

        // Select an action from Wheel of Destruction
        // this line as one above randomly picks 21 possible actions between 0 and 20
        // some actions do nothing but others kill DC's or delete the queue
        int action = rand() % 21;

        //this are the options that do nothing
        if (action == 0 || action == 8 || action == 19) {
            log_event("DX chose to do nothing.");
        }
        else if (action == 10 || action == 17) {
            // this are the options that delete message queue
            if (msgctl(sharedMemory->msgQueueID, IPC_RMID, NULL) == -1) {
                log_event("DX failed to delete message queue.");
            }
            else {
                log_event("DX deleted the message queue.");
            }
        }
        else {
            // This are the options that kill the DC process
            int targetIndex = (action - 1) % MAX_DC_ROLES;
            if (sharedMemory->dc[targetIndex].dcProcessID > 0) {
                if (kill(sharedMemory->dc[targetIndex].dcProcessID, SIGHUP) == 0) {
                    char logMsg[100];
                    snprintf(logMsg, sizeof(logMsg), "DX killed DC-%d (PID %d)",
                        targetIndex + 1, sharedMemory->dc[targetIndex].dcProcessID);
                    log_event(logMsg);
                }
                else {
                    log_event("DX failed to kill DC process (may not exist).");
                }
            }
            else {
                log_event("DX found no valid DC process to kill.");
            }
        }
    }

    return EXIT_SUCCESS;
}