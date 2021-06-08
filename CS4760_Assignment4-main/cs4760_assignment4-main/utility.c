// Name: Aj Nobs
// Date: 03/22/21
// Course: CS 4760
// Project: Assignment 4
// File Name: utility.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Simulates an operating system scheduler. Randomly creates a child process and uses
//				a message queue alongside shared memory to keep the processes in sync. Logging is done
//				along with tracking of system resource such as time and current process running.
//-----------------------------------------------------------------------------------------------------

#include "utility.h"

/* Allocate and attach shared memory */
void scheduleShm() {
	skdId = shmget(SKD_SHM_KEY, (1200), SHM_FLAGS); // Create shared memory ID for schedule info
	if (skdId == -1) { errorExit(progName, "skdId - shmget"); }

	skdInfo = shmat(skdId, NULL, 0); // Attach to shared memory for schedule info
	if (skdInfo == (void *) -1) { errorExit(progName, "skdInfo - shmat"); }
}

/* Allocate and attach shared memory */
void processTableShm() {
	ptId = shmget(PT_SHM_KEY, (500), SHM_FLAGS); // Create shared memory ID for process table
	if (ptId == -1) { errorExit(progName, "ptId - shmget"); }

	pTable = shmat(skdId, NULL, 0); // Attach to shared memory for process table
	if (pTable == (void *) -1) { errorExit(progName, "pTable - shmat"); }
}

/* Schedule Info Initializer */
void createSchedule() {
	int i; // Counter variable

	/* Allocate and attach shared memory */
	scheduleShm();
	/*
	skdId = shmget(SKD_SHM_KEY, (1200), SHM_FLAGS); // Create shared memory ID for schedule info
	if (skdId == -1) { errorExit(progName, "skdId - shmget"); }
	skdInfo = shmat(skdId, NULL, 0); // Attach to shared memory for schedule info
	if (skdInfo == (void *) -1) { errorExit(progName, "skdInfo - shmat"); }
	*/

	/* Initialize all schedule variables */
	skdInfo->sClock = 0;
	skdInfo->nsClock = 0;
	skdInfo->cpuTime = 0;
	skdInfo->totalTime = 0;
	skdInfo->burstTime = 0;
	skdInfo->ossPid = getpid();
	skdInfo->localPid = getpid();
	skdInfo->bitVec[0] = 1;
	for (i = 1; i < 20; i++) {
		skdInfo->bitVec[i] = 0;
	}
	skdInfo->i = 0;
}

/* Process Table Initializer */
void createProcessTable() {
	int i; // Counter variable

	/* Allocate and attach shared memory */
	processTableShm();
	/*
	ptId = shmget(PT_SHM_KEY, (500), SHM_FLAGS); // Create shared memory ID for process table
	if (ptId == -1) { errorExit(progName, "ptId - shmget"); }
	pTable = shmat(skdId, NULL, 0); // Attach to shared memory for process table
	if (pTable == (void *) -1) { errorExit(progName, "pTable - shmat"); }
	*/

	/* Initialize OSS Process */
	pTable->pcb[0].pid = getpid(); /*
	pTable->pid[0] = pTable->pcb->pid;
	skdInfo->sClock = 0;
	*/
	pTable->pcb[0].pType = 'o';
	pTable->pcb[0].pState = 'r';
	pTable->pcb[0].burstTime = 1;

	/* Initialize User Processes */
	for (i=1; i<20; i++) {
		pTable->pcb[i].pid = 0;
		pTable->pcb[i].pType = 'u';
		pTable->pcb[i].pState = 'u';
		pTable->pcb[i].burstTime = 0;
	}
}

/* Message Queue Initializer */
void createMessageQueue() {
	msqId = msgget(MSG_KEY, SHM_FLAGS); // Create shared memory ID for message queue
	if (msqId == -1) { errorExit(progName, "msqId - msgget"); }
}

// Logical Clock Increment Function
void clockInc (unsigned int increment) {
	skdInfo->nsClock += (increment * TCK); // Increment

	while (skdInfo->nsClock >= (unsigned int) S_NS) { // Roll nanoseconds to seconds if threshold is met, 1 sec == 1,000,000,000 ns
		skdInfo->sClock += 1;
		skdInfo->nsClock -= S_NS;
	}
}

// Print Logical Clock
void logger(char *pName, char *str, int pid) {
	char buf[BUF_SIZE];
	logLines++;
	if (logLines >= 10000 && lFlag == 0) {
		printf("Too many lines in logfile [%d] \n", logLines);
		errorExit(progName, "Logger - Exceeded Line Limit");
	}
	// More readable program names

	/*
	if (strcmp(pName, "./oss") == 0) {
		strcpy(pName, "OSS");
	}
	if (strcmp(pName, "./user") == 0) {
		strcpy(pName, "User");
	}
	*/

	/* Print to Screen */
	snprintf(buf, BUF_SIZE, "%3u:%9u", skdInfo->sClock, skdInfo->nsClock);
	printf("%s | ", buf);
	snprintf(buf, BUF_SIZE, "%4s (%5i | %5i)", pName, pid, getppid());
	printf("%-18s |", buf);
	printf(" %s \n", str);
	/*
	snprintf(buf, BUF_SIZE, "%3i:%9i", skdInfo->sClock, skdInfo->nsClock);
	printf("%s | ", buf);
	snprintf(buf, BUF_SIZE, "%4s (%i | %i)", "User", 11235, pid);
	printf("%-18s |", buf);
	printf(" Message from the User process \n");
	*/

	/* Print to Logfile */
	snprintf(buf, BUF_SIZE, "%3u:%9u", skdInfo->sClock, skdInfo->nsClock);
	fprintf(lFile, "%s | ", buf);
	snprintf(buf, BUF_SIZE, "%4s (%5i | %5i)", pName, pid, getppid());
	fprintf(lFile, "%-18s |", buf);
	fprintf(lFile, " %s \n", str);
}

// Print Schedule Info
void printSkd() {
	int i;
	printf("Clock: %u:%u \n", skdInfo->sClock, skdInfo->nsClock);
	printf("CPU Time: %i \n", skdInfo->cpuTime);
	printf("Total Time: %i \n", skdInfo->totalTime);
	printf("Burst Time: %i \n", skdInfo->burstTime);
	printf("OSS PID: %i \n", skdInfo->ossPid);
	printf("Local PID: %i \n", skdInfo->localPid);
	printf("Bit Vector: ");
	for (i=0; i<19; i++) {
		printf("%i, ", skdInfo->bitVec[i]);
	}
	printf("%i \n", skdInfo->bitVec[19]);
}

// Print Bit Vector
void printBitVec() {
	int i;
	printf("Bit Vector: <");
	for (i=0; i<19; i++) {
		printf("%i, ", skdInfo->bitVec[i]);
	}
	printf("%i> \n", skdInfo->bitVec[19]);
}

// Perror and Exit Function
void errorExit (char *pName, char *str) {
	int memDtDl = 0;
	char buf[1024];

	snprintf(buf, 1024, "%s: %s", pName, str);
	perror(buf);

	if (skdInfo != (void *) -1) { // Detach and Deallocate skdInfo
		// Detach
		memDtDl = shmdt(skdInfo);
		if (memDtDl != 0) { perror("skdInfo - shmdt"); }
		memDtDl = 0;
		// Deallocate
		memDtDl = shmctl(skdId, IPC_RMID, NULL);
		if (memDtDl != 0) { perror("skdInfo - shmctl"); }
		memDtDl = 0;
	}
	if (pTable != (void *) -1) { // Detach and Deallocate pTable
		// Detach
		memDtDl = shmdt(pTable);
		if (memDtDl != 0) { perror("pTable - shmdt"); }
		memDtDl = 0;
		// Deallocate
		memDtDl = shmctl(ptId, IPC_RMID, NULL);
		if (memDtDl != 0) { perror("pTable - shmctl"); }
		memDtDl = 0;
	}
	if (msqId != -1) { // Deallocate msq
		memDtDl = msgctl(msqId, IPC_RMID, NULL);
		if (memDtDl != 0) { perror("msq - msgctl"); }
		memDtDl = 0;
	}

	printf("All detached and deallocated on error \n");

	exit(EXIT_FAILURE);
}

// Add process to process table
void addProcess (int i, pid_t pid, char pType, char pState) {
	skdInfo->i = i;
	pTable->pcb[i].pid = pid;
	pTable->pcb[i].pType = pType;
	pTable->pcb[i].pState = pState;
	pTable->pcb[i].burstTime = 0;
}

// Remove process to process table
void remProcess(int i) {
	skdInfo->burstTime = 0;
	pTable->pcb[i].pid = 0;
	pTable->pcb[i].pType = 'u';
	pTable->pcb[i].pState = 'u';
	pTable->pcb[i].burstTime = 0;
}

// Switch process in process table to wait or run
void pStateSwitch(int i, int j) {
	// First process to switch
	if (pTable->pcb[i].pState == 'w') {
		pTable->pcb[i].pState = 'r';
		skdInfo->bitVec[i] = 1;
	} else if (pTable->pcb[i].pState == 'r') {
		pTable->pcb[i].pState = 'w';
		skdInfo->bitVec[i] = 0;
	}

	// Second process to switch
	if (pTable->pcb[j].pState == 'w') {
		pTable->pcb[j].pState = 'r';
		skdInfo->bitVec[j] = 1;
	} else if (pTable->pcb[j].pState == 'r') {
		pTable->pcb[j].pState = 'w';
		skdInfo->bitVec[j] = 0;
	}
}

