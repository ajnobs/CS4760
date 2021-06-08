// Name: Aj Nobs
// Date: 03/06/21
// Course: CS 4760
// Project: Assignment 3
// File Name: statMon.c
// Project Files: monitor.c monitor.h statMon.c statMon.h consumer.c consumer.h producer.c producer.h Makefile README
// Description:
//
//
//
//-----------------------------------------------------------------------------------------------------

#include <getopt.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "statMon.h"




void resetBuf(int argc, char **argv) {
	progName = argv[0];

	setvbuf(stdout, NULL, _IONBF, 0);

	setvbuf(stderr, NULL, _IONBF, 0);
}



void setpidTemp(pid_t pid) {
	shmPtr->pidTemp = pid;
}



pid_t getpidTemp() {
	return shmPtr->pidTemp;
}



void memAlloc(bool isAlloc) {
	shmKey = ftok("monitor.c", 1);
	if (shmKey == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "ftok");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}

	shmId = shmget(shmKey, sizeof(struct shm), IPC_CREAT | 0666);
	if (shmId == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s %d", progName, "memAlloc - shmget", shmId);
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	} else {
		shmPtr = (struct shm*) shmat(shmId, NULL, 0);
	}

	if (isAlloc) {
		// logger("logfile", "Memory allocated\n");
		printf("\nMemory allocated ");
	}
}



void memDealloc() {
	if (shmPtr != NULL && shmdt(shmPtr) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "memDealloc - shmdt");
		perror(buf);

		// memDealloc();
		// semDealloc();

		exit(EXIT_FAILURE);
	}

	if (shmId > 0) {
		if (shmctl(shmId, IPC_RMID, NULL) == -1) { // Error & Exit
			char buf[1024];

			snprintf(buf, 1024, "%s: %s", progName, "memDealloc - shmctl");
			perror(buf);

			memDealloc();
			semDealloc();

			exit(EXIT_FAILURE);
		}

		// logger("logfile", "Memory deallocated\n");
		printf("\nMemory deallocated ");
	}
}



void semAlloc(bool isAlloc, ...) {
	int num = n + m;

	semKey = ftok("monitor.c", 2);
	if (semKey == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "semAlloc - ftok");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}

	semId = semget(semKey, num, IPC_CREAT | 0666);
	if (semId == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "semAlloc - semget");
		perror(buf);

		// memDealloc();
		// semDealloc();

		exit(EXIT_FAILURE);
	}

	if (isAlloc) {
		int i;
		for (i = 0; i < num; i++) {
			if (semctl(semId, i, SETVAL, 1) == -1) { // Error & Exit
				char buf[1024];

				snprintf(buf, 1024, "%s: %s", progName, "semAlloc - semctl");
				perror(buf);

				memDealloc();
				semDealloc();

				exit(EXIT_FAILURE);
			}
			printf("\nSemaphore %d allocated ", i);
		}

		// logger("logfile", "Semaphores allocated\n");
		printf("\nSemaphores allocated ");
	}
}



void semDealloc() {
	if (semId > 0) {
		if (semctl(semId, 0, IPC_RMID) == -1) { // Error & Exit
			char buf[1024];

			snprintf(buf, 1024, "%s: %s", progName, "semDealloc - semctl");
			perror(buf);

			// memDealloc();
			// semDealloc();

			exit(EXIT_FAILURE);
		}

		// logger("logfile", "%s: Semaphores deallocated\n");
		printf("\nSemaphores deallocated ");
	}
}



void semWait(int num) {
	sop.sem_num = num;
	sop.sem_op = -1;
	sop.sem_flg = 0;

	if (semop(semId, &sop, 1) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "semWait - semop");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}
}



void semSignal(int num) {
	sop.sem_num = num;
	sop.sem_op = 1;
	sop.sem_flg = 0;

	if (semop(semId, &sop, 1) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "semSignal - semop");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}
}



void sigAct(int signum, void handler(int)) {
	struct sigaction sa;

	if (sigemptyset(&sa.sa_mask) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "sigemptyset");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}

	sa.sa_handler = handler;
	sa.sa_flags = 0;

	if (sigaction(signum, &sa, NULL) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "sigAct - sigaction");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}
}



void logger(char *fmt, ...) {
	// logFile = fopen(logName, "w+");

	if (logFile == NULL) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "logger - fopen");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}

	char buf[1024];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, 1024, fmt, args);
	va_end(args);

	fprintf(logFile, buf);
	fclose(logFile);
}
