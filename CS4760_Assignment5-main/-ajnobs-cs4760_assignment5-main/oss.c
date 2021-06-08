// Name: Aj Nobs
// Date: 04/20/21
// Course: CS 4760
// Project: Assignment 5
// File Name: oss.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Implementation of a resource management module for our Operating System Simulator OSS.
//-----------------------------------------------------------------------------------------------------

#include "utility.h"
#include "oss.h"

int main(int argc, char *argv[]) {
	progName = "OSS";
	char   buf[BUF_SIZE];
	int    opt; // Command Line Options
	char   *path = realpath("user", NULL); // Path to user program executable
	int    memDtDl = 0; // Memory deletion and detachment error flag
	int    randomVal; // Holds random value
	srand((unsigned) ti(NULL)); // Rand initialization
	int i = 0; // Generic count variables
	int count = 0;
	logLines = 0;
	lFlag = 0;

	// Get command line argument options
	while(true) {
		opt = getopt(argc, argv, "h");
		if (opt == -1) { break; } // Exit loop

		switch (opt) {
			case 'h': // Print monitor help page
				printf("\nNAME \n     oss - simulates operating system resource management ");
				printf("\nUSAGE \n     oss [-h]  ");
				printf("\nDESCRIPTION \n     Simulates the method of management and control used by the OS.");
				printf("\nOPTIONS \n     -h, help \n              display this help and exit ");
				printf("\nAUTHOR ");
				printf("\n     Written by Aj Nobs\n\n");
				exit(EXIT_SUCCESS);


			default: // Unexpected exit
				perror("Unexpected error during parsing options");
				exit(EXIT_FAILURE); // return -1;
		} // End of CL Option switch
	} // End of CL Option while loop



	// Open logfile
	lFile = fopen(logName, "w+");
	if (lFile == NULL) { errorExit(progName, "fopen"); }
	fprintf(lFile, "[ASSIGNMENT 5 LOGFILE]____________________________________________________________________________________________________ \n");

	createClock();
	clockInc(1000);
	logger("Schedule Info created and initialized ", 1, 0);

	createResource();
	clockInc(1000);
	logger("Process Table created and initialized ", 1, 0);

	createMessageQueue();
	msg.type = 1;
	clockInc(500);
	logger("Message Queue created ", 1, 0);

	pid_t fPid;

	while (clck->sClock < t && logLines < 100000) {
		fPid = fork();

		if (i >= 19) { i = 1; } // Start at beginning of queue
		randomVal = rand() % 900 + 100;
		clockInc(randomVal);

		if (fPid == 0) { // Child Process
			clockInc(40);
			logger("Exec User Process", 1, 0);

			execl(path, path, "User", logName, (char *) NULL);

			printf("Whoops \n");
			perror("User failed to run ");
			exit(EXIT_FAILURE);
		} else { // Parent process
			i++;
			count++;
			clockInc(10);
			// logger("Forked User process ", 1, 0);

			randomVal = rand() % 2;
			if (randomVal > 1) {
				addProcess(i, fPid, 'c', 'w');
			} else {
				addProcess(i, fPid, 'i', 'w');
			}

			clockInc(10);
			snprintf(buf, BUF_SIZE, "Process %i added to queue", i);
			logger(buf, getpid());

			pStateSwitch(0, i);
			clockInc(20);
			// logger("Switching to new process ", 1, 0);

			msg.text[0] = 'r';
			msgsnd(msqId, &msg, sizeof(msg), 0);

			wait(NULL);

			clck->localPid = getpid();
			pStateSwitch(0, i);
			clockInc(clck->burstTime);
			// logger("Returned to OSS ", 1, 0);
			snprintf(buf, BUF_SIZE, "Process %i Burst Time: %i", i, clck->burstTime);
			logger(buf, getpid());

			msgrcv(msqId, &msg, sizeof(msg), 1, 0);
			snprintf(buf, BUF_SIZE, "Message: %s", msg.text);
			// logger(buf, getpid());
			// printf("Message:%s \n", msg.text);

			remProcess(i);
			clockInc(10);
			logger("Removed child process", 1, 0);



			// printSkd();
			// printBitVec();
		}
	}

	logger("Time limit reached ", 1, 0);
	printf("\nLines in Logfile: %i \n", logLines);

	printf("Total Number of Processes = %i \n", count);
	printf("Total Time = %u:%u \n", clck->sClock, clck->nsClock);


	/* Detach and deallocate all shared memory in use */
	if (clck != (void *) -1) { // Detach and Deallocate clck
			// Detach
			memDtDl = shmdt(clck);
			if (memDtDl != 0) { perror("clck - shmdt"); }
			printf("\nclck Detached . . . ");
			memDtDl = 0;
			// Deallocate
			memDtDl = shmctl(clckId, IPC_RMID, NULL);
			printf("Deallocated \n");
			if (memDtDl != 0) { perror("clck - shmctl"); }

			memDtDl = 0;
		}
	if (rTable != (void *) -1) { // Detach and Deallocate rTable
			// Detach
			memDtDl = shmdt(rTable);
			if (memDtDl != 0) { perror("rTable - shmdt"); }
			printf(" rTable Detached . . . ");
			memDtDl = 0;
			// Deallocate
			memDtDl = shmctl(rtId, IPC_RMID, NULL);
			if (memDtDl != 0) { perror("rTable - shmctl"); }
			printf("Deallocated \n");
			memDtDl = 0;
		}
	if (msqId != -1) { // Deallocate msq
			memDtDl = msgctl(msqId, IPC_RMID, NULL);
			if (memDtDl != 0) { perror("msq - msgctl"); }
			printf("    msq Detached \n");
			memDtDl = 0;
		}

	fclose(lFile);
	free(path);

	return 0;
}
