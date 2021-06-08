// Name: Aj Nobs
// Date: 04/20/21
// Course: CS 4760
// Project: Assignment 5
// File Name: utility.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Implementation of a resource management module for our Operating System Simulator OSS.
//-----------------------------------------------------------------------------------------------------

#include "utility.h"

/* Allocate and attach shared memory */
void clockShm() {
	clckId = shmget(CLCK_SHM_KEY, (1200), SHM_FLAGS); // Create shared memory ID for schedule info
	if (clckId == -1) { errorExit(progName, "clckId - shmget"); }

	clck = shmat(clckId, NULL, 0); // Attach to shared memory for schedule info
	if (clck == (void *) -1) { errorExit(progName, "clck - shmat"); }
}

/* Allocate and attach shared memory */
void resourceShm() {
	rtId = shmget(RSC_SHM_KEY, (500), SHM_FLAGS); // Create shared memory ID for process table
	if (rtId == -1) { errorExit(progName, "rtId - shmget"); }

	rTable = shmat(clckId, NULL, 0); // Attach to shared memory for process table
	if (rTable == (void *) -1) { errorExit(progName, "rTable - shmat"); }
}

/* Clock Initializer */
void createClock() {
	/* Allocate and attach shared memory */
	clockShm();

	/* Initialize all clock variables */
	clck->sClock = 0;
	clck->nsClock = 0;
	clck->i = 0;
}

/* Resource Table Initializer */
void createResource() {
	int i, j, n;
	char buf[BUF_SIZE];

	srand((unsigned) ti(NULL));

	/* Allocate and attach shared memory */
	resourceShm();

	/* Initialize Resources */
	for (i=0; i<20; i++) {
		snprintf(buf, BUF_SIZE, "r%d", i);
		rTable->rd[i].name = buf;
		n = (rand() % 10)+1;
		rTable->rd[i].num = n;
		rTable->rd[i].used = 0;
		for (j=0; j<n; ++j) {
			rTable->rd[i].uProc[j] = 0;
		}
		rTable->rd[i].req = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	}
}

/* Message Queue Initializer */
void createMessageQueue() {
	msqId = msgget(MSG_KEY, SHM_FLAGS); // Create shared memory ID for message queue
	if (msqId == -1) { errorExit(progName, "msqId - msgget"); }
}

// Logical Clock Increment Function
void clockInc (unsigned int increment) {
	clck->nsClock += (increment * TCK); // Increment

	while (clck->nsClock >= (unsigned int) S_NS) { // Roll nanoseconds to seconds if threshold is met, 1 sec == 1,000,000,000 ns
		clck->sClock += 1;
		clck->nsClock -= S_NS;
	}
}

// Print Logical Clock
void logger(char *str, int incTime, int tabStart) {
	char buf[BUF_SIZE];
	logLines++;
	if (logLines >= 100000 && lFlag == 0) {
		printf("Too many lines in logfile [%d] \n", logLines);
		errorExit(progName, "Logger - Exceeded Line Limit");
	}
	/*
	// More readable program names
	if (strcmp(pName, "./oss") == 0) {
		strcpy(pName, "OSS");
	}
	if (strcmp(pName, "./user") == 0) {
		strcpy(pName, "User");
	}
	*/

	// Initial tab
	if (tabStart == 1) {
		printf("\t");
		fprintf(lFile, "\t");
	}

	// Print string to Screen & Logfile
	printf("%s ", str);
	fprintf(lFile, "%s ", str);


	// Timestamp
	if (incTime == 1) {
		snprintf(buf, BUF_SIZE, "%3u:%9u", clck->sClock, clck->nsClock);
		printf("at time %s \n", buf);
		fprintf(lFile, "at time %s \n", buf);
	} else {
		printf(" \n");
		fprintf(lFile, " \n");
	}


}

// Print Schedule Info
void printInfo() {
	int i;
	printf("Clock: %u:%u \n", clck->sClock, clck->nsClock);

	printf("Resources: ");
	for (i=0; i<19; i++) {
		printf("%i, ", clck->bitVec[i]);
	}
	printf("%i \n", clck->bitVec[19]);
}

// Perror and Exit Function
void errorExit (char *pName, char *str) {
	int memDtDl = 0;
	char buf[1024];

	snprintf(buf, 1024, "%s: %s", pName, str);
	perror(buf);

	if (clck != (void *) -1) { // Detach and Deallocate clck
		// Detach
		memDtDl = shmdt(clck);
		if (memDtDl != 0) { perror("clck - shmdt"); }
		memDtDl = 0;
		// Deallocate
		memDtDl = shmctl(clckId, IPC_RMID, NULL);
		if (memDtDl != 0) { perror("clck - shmctl"); }
		memDtDl = 0;
	}
	if (rTable != (void *) -1) { // Detach and Deallocate rTable
		// Detach
		memDtDl = shmdt(rTable);
		if (memDtDl != 0) { perror("rTable - shmdt"); }
		memDtDl = 0;
		// Deallocate
		memDtDl = shmctl(rtId, IPC_RMID, NULL);
		if (memDtDl != 0) { perror("rTable - shmctl"); }
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

}

// Remove process to process table
void remProcess(int i) {

}

// Switch process in process table to wait or run
void pStateSwitch(int i, int j) {
	// First process to switch
	if (rTable->rd[i].pState == 'w') {
		rTable->rd[i].pState = 'r';
		clck->bitVec[i] = 1;
	} else if (rTable->rd[i].pState == 'r') {
		rTable->rd[i].pState = 'w';
		clck->bitVec[i] = 0;
	}

	// Second process to switch
	if (rTable->rd[j].pState == 'w') {
		rTable->rd[j].pState = 'r';
		clck->bitVec[j] = 1;
	} else if (rTable->rd[j].pState == 'r') {
		rTable->rd[j].pState = 'w';
		clck->bitVec[j] = 0;
	}
}

