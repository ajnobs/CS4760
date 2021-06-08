// Name: Aj Nobs
// Date: 03/06/21
// Course: CS 4760
// Project: Assignment 3
// File Name: statMon.h
// Project Files: monitor.c monitor.h statMon.c statMon.h consumer.c consumer.h producer.c producer.h Makefile README
// Description:
//
//
//
//-----------------------------------------------------------------------------------------------------

#ifndef CS4760_ASSIGNMENT3_STATMON_H
#define CS4760_ASSIGNMENT3_STATMON_H

static char *progName = NULL; // Program Name
static FILE *logFile = NULL; // Program File
static char *logName  = NULL; // Logfile Name
static int m = 2; // Num Producer
static int n = 6; // Num Consumer
static int t = 100; // Time
static int maxProcess = 20; // Max number of processes

void resetBuf(int argc, char **argv);

void memDealloc();
void semDealloc();

void logger(char *fmt, ...);

struct shm {
	// Items go here
	pid_t pidTemp;
};

static key_t shmKey;
static int shmId;
static struct shm *shmPtr = NULL;

void setpidTemp(pid_t pid);

pid_t getpidTemp();

void memAlloc(bool isAlloc);


static key_t semKey;
static int semId;
static struct sembuf sop;

void semAlloc(bool isAlloc, ...);

void semWait(int num);

void semSignal(int num);

void sigAct(int signum, void handler(int));

// void rtouch(char *path);

#endif //CS4760_ASSIGNMENT3_STATMON_H
