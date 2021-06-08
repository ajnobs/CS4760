// Name: Aj Nobs
// Date: 03/22/21
// Course: CS 4760
// Project: Assignment 4
// File Name: utility.h
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Simulates an operating system scheduler. Randomly creates a child process and uses
//				a message queue alongside shared memory to keep the processes in sync. Logging is done
//				along with tracking of system resource such as time and current process running.
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

#define SKD_SHM_KEY 0x1123
#define PT_SHM_KEY 0x5813
#define MSG_KEY 0x2134
#define SHM_FLAGS (IPC_CREAT | 0666)
#define BUF_SIZE 1024
#define S_NS 1000000000 /* 1 second == 1,000,000,000 nanoseconds */
#define TCK 10000 /* Amount of time to add after each event == 10,000 ns */
#define QNTM 10
#define maxTimeBetweenNewProcsNS 5
#define maxTimeBetweenNewProcsSecs 1000

char *progName; // Program Name
int t; // Time variable
char *f; // Log name from CLI
char *logName; // Log name with .log suffix
FILE *lFile; // Log file
int logLines; // Lines in the log file
int lFlag;

int arrivalTime;
int burstTime;
int waitTime;

// Allocate and attach shared memory for ipc information
void scheduleShm();

// Allocate and attach shared memory for process table
void processTableShm();

// Schedule Information Structure
struct scheduleInfo {
	unsigned int sClock; // Simulated clock seconds variable
	unsigned int nsClock; // Simulated clock nanoseconds variable
	int cpuTime;
	int totalTime;
	int burstTime;
	pid_t ossPid;
	pid_t localPid;
	bool bitVec[20];
	int i;
};
typedef struct scheduleInfo SKDINFO;
// Schedule Info Shared Memory
int skdId;
SKDINFO *skdInfo;

// Schedule Info Initializer
void createSchedule();

// Process Control Block Structure
struct processControlBlock {
	pid_t pid;
	char pType; // c for CPU, i for I/O, o for OSS, u for Unused
	char pState; // r for running, w for waiting, u for Unused
	int burstTime; // Number of tck process accumulated
};
typedef struct processControlBlock PCB;
// Process Table Structure
struct processTable {
	pid_t pid[20];
	PCB pcb[20]; // Process Control Block
};
typedef struct processTable PTABLE;
// Process Table Shared Memory
int ptId;
PTABLE *pTable;

// Process Table Initializer
void createProcessTable();

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

// Print Schedule Info
void printSkd();

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