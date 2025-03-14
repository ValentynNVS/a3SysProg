#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "../../Common/inc/MessageQueue.h"  // Include the shared header file

#define MIN_WAIT 10
#define MAX_WAIT 30

// Possible machine status messages
const char *statusDescriptions[] = {
    "Everything is OKAY",
    "Hydraulic Pressure Failure",
    "Safety Button Failure",
    "No Raw Material in the Process",
    "Operating Temperature Out of Range",
    "Operator Error",
    "Machine is Off-line"
};

// Define status codes
#define OKAY 0
#define OFFLINE 6

int main() {
    key_t uniqueToken = getUniqueToken("/home", kMessageQueueSeed);
    int msgid;

    // Declare the 'now' variable of type time_t
    time_t now;

    // Wait until message queue exists
    while ((msgid = messageQueueExists(uniqueToken)) == -1) {
        sleep(10);
    }

    // Prepare message structure
    QueueMessage msg;
    msg.type = 1;  // Message type (unused in this implementation)
    msg.machineID = getpid();
    
    srand(time(NULL) ^ msg.machineID);  // Seed random generator

    // Send initial "Everything is OKAY" message
    msg.statusCode = OKAY;
    snprintf(msg.statusDescription, kGeneralStringLength, "%s", statusDescriptions[OKAY]);
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);



// everything seems to be okay to that point, i need to test those functions,
//also, i changed the directory path to adhere to the requirements but i had my own path before, so it could be a problem if anyone wants to test it
//cuz i havent changed it yet
    // Log message
    FILE *logFile = fopen("/tmp/dataCreator.log", "a");
    if (logFile) {
        // Prepare timestamp in the format [YYYY-MM-DD HH:MM:SS]
        char timestamp[20];
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        fprintf(logFile, "[%s] : DC [%d] – MSG SENT – Status %d (%s)\n",
                timestamp, msg.machineID, msg.statusCode, msg.statusDescription);
        fclose(logFile);
    }

    // Main loop to send random status messages
    while (1) {
        sleep(rand() % (MAX_WAIT - MIN_WAIT + 1) + MIN_WAIT);

        msg.statusCode = (rand() % 6) + 1;  // Randomly pick a status
        snprintf(msg.statusDescription, kGeneralStringLength, "%s", statusDescriptions[msg.statusCode]);

        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        // Log message
        logFile = fopen("/tmp/dataCreator.log", "a");
        if (logFile) {
            // Prepare timestamp in the format [YYYY-MM-DD HH:MM:SS]
            char timestamp[20];
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

            fprintf(logFile, "[%s] : DC [%d] – MSG SENT – Status %d (%s)\n",
                    timestamp, msg.machineID, msg.statusCode, msg.statusDescription);
            fclose(logFile);
        }

        if (msg.statusCode == OFFLINE) {
            break;  // Exit if machine goes offline
        }
    }

    return 0;
}
