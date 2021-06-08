// Name: Aj Nobs
// Date: 04/20/21
// Course: CS 4760
// Project: Assignment 5
// File Name: utility.h
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Implementation of a resource management module for our Operating System Simulator OSS.
//-----------------------------------------------------------------------------------------------------


#ifndef CS4760_ASSIGNMENT4_UTILITY_H
#define CS4760_ASSIGNMENT4_UTILITY_H

#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CLCK_SHM_KEY 0x1123
#define RSC_SHM_KEY 0x5813
#define MSG_KEY 0x2134
#define SHM_FLAGS (IPC_CREAT | 0666)
#define BUF_SIZE 1024
#define S_NS 1000000000 /* 1 second == 1,000,000,000 nanoseconds */
#define TCK 1000000 /* Amount of time to add after each event == 1,000,000 ns */
#define maxTimeBetweenNewProcsNS 5
#define maxTimeBetweenNewProcsSecs 1000

char *progName; // Program Name
int t = 5; // Time variable
time_t ti; // Rand seed
char *logName = "logfile.log"; // Log name
FILE *lFile; // Log file
int logLines; // Lines in the log file
int lFlag; // Log flag


// Allocate and attach shared memory for ipc information
void Shm();

// Allocate and attach shared memory for resource table
void resourceShm();

// Clock Structure
struct shClock {
	unsigned int sClock; // Simulated clock seconds variable
	unsigned int nsClock; // Simulated clock nanoseconds variable
	int i;
};
typedef struct shClock SHCLOCK;
// Clock Shared Memory
int clckId;
SHCLOCK *clck;

// Clock Initializer
void createClock();

// Resource Descriptor Structure
struct resourceDescriptor {
	char* name; // resource name
	int num; // Number of a resource
	int used; // Number of resource in use
	pid_t uProc[num]; // What process is using this resource
	pid_t req[20]; // Requests for allocation
};
typedef struct resourceDescriptor RD;
// Resource Table Structure
struct resourceTable {
    int open[20]; // Open resource count
	RD rd[20]; // Resource Descriptors
};
typedef struct resourceTable RTABLE;
// Resource Table Shared Memory
int rtId;
RTABLE *rTable;

// Resource Table Initializer
void createResource();

struct msgbuf {
	long type;
	char text[100];
} msg;
// Message Queue Shared Memory
int msqId;
// Message Queue Initializer
void createMessageQueue();

// Logical Clock Increment
void clockInc(unsigned int increment);

// Print current clock time and what called it
void logger(char *pName, char *str, int pid);

// Print Info
void printInfo();

// Print Bit Vector
void printBitVec();

// Error & Exit
void errorExit (char *pName, char *str);

// Add process to process table
void addProcess (int i, pid_t pid, char pType, char pState);

// Remove process from process table
void remProcess(int i);

// Edit process in process table
void pStateSwitch(int i, int j);


#endif //CS4760_ASSIGNMENT4_UTILITY_H