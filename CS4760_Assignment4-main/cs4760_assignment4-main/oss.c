// Name: Aj Nobs
// Date: 03/22/21
// Course: CS 4760
// Project: Assignment 4
// File Name: oss.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Simulates an operating system scheduler. Randomly creates a child process and uses
//				a message queue alongside shared memory to keep the processes in sync. Logging is done
//				along with tracking of system resource such as time and current process running.
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
	time_t ti; // Rand seed
	srand((unsigned) time(&ti)); // Rand initialization
	int i = 0; // Generic count variables
	int count = 0;


	// Get command line argument options
	while(true) {
		opt = getopt(argc, argv, "hs:l:");
		if (opt == -1) { break; } // Exit loop

		switch (opt) {
			case 'h': // Print monitor help page
				printf("\nNAME \n     oss - simulates operating system scheduler ");
				printf("\nUSAGE \n     oss [-h] [-s t] [-l f]  ");
				printf("\nDESCRIPTION \n     Simulates the method of management and control used by the OS.");
				printf("\nOPTIONS \n     -h, help \n              display this help and exit ");
				printf("\n     -s t, time before termination \n              Indicate how many maximum seconds before the system terminates ");
				printf("\n     -l f, logfile name \n              set the number of consumers, default: n=6 ");
				printf("\nAUTHOR ");
				printf("\n     Written by Aj Nobs\n\n");
				exit(EXIT_SUCCESS);


			case 's': // Set time until termination
				if (!isdigit(*optarg)) { // Number check t
					perror("Invalid t value (Non-Number)");
					exit(EXIT_FAILURE);
				}
				t = atoi(optarg); // Assign t
				if (t <= 0) { // Range check t
					perror("Invalid t value (Less than 0)");
					exit(EXIT_FAILURE);
				}
				break;


			case 'l': // Set logfile name
				if ((f = optarg) == NULL) {
					perror("Failed to create logfile");
					exit(EXIT_FAILURE);
				}
				break;


			default: // Unexpected exit
				perror("Unexpected error during parsing options");
				exit(EXIT_FAILURE); // return -1;
		} // End of CL Option switch
	} // End of CL Option while loop

	// Set logfile name
	if (t <= 0) { t = 3; }
	if (f == NULL) { f = "logfile"; }
	if ((logName = malloc(strlen(f) + strlen(".log") + 1)) != NULL) {
		logName[0] = '\0';
		strcat(logName, f);
		strcat(logName, ".log");
	}
	logLines = 0;
	lFlag = 0;
	printf("\nParameters provided: \nMax Exec Time(s) = %d \nLogfile Name(f) = \"%s\" \n\n", t, logName);

	// Open logfile
	lFile = fopen(logName, "w+");
	if (lFile == NULL) { errorExit(progName, "fopen"); }
	printf(        "__Time(s:ns)__|_Program (PID | PPID)_|________________________________________Message________________________________________ \n");
	fprintf(lFile, "__Time(s:ns)__|_Program (PID | PPID)_|________________________________________Message________________________________________ \n");

	createSchedule();
	clockInc(1000);
	logger(progName, "Schedule Info created and initialized ", getpid());

	createProcessTable();
	clockInc(1000);
	logger(progName, "Process Table created and initialized ", getpid());

	createMessageQueue();
	msg.type = 1;
	clockInc(500);
	logger(progName, "Message Queue created ", getpid());

	pid_t fPid;

	while (skdInfo->sClock < t) {
		fPid = fork();

		if (i >= 19) { i = 1; } // Start at beginning of queue
		randomVal = rand() % 900 + 100;
		clockInc(randomVal);

		if (fPid == 0) { // Child Process
			clockInc(40);
			logger(progName, "Exec User Process", getpid());

			execl(path, path, "User", logName, (char *) NULL);

			printf("Whoops \n");
			perror("User failed to run ");
			exit(EXIT_FAILURE);
		} else { // Parent process
			i++;
			count++;
			clockInc(10);
			// logger(progName, "Forked User process ", getpid());

			randomVal = rand() % 2;
			if (randomVal > 1) {
				addProcess(i, fPid, 'c', 'w');
			} else {
				addProcess(i, fPid, 'i', 'w');
			}

			clockInc(10);
			snprintf(buf, BUF_SIZE, "Process %i added to queue", i);
			logger(progName, buf, getpid());

			pStateSwitch(0, i);
			clockInc(20);
			// logger(progName, "Switching to new process ", getpid());

			msg.text[0] = 'r';
			msgsnd(msqId, &msg, sizeof(msg), 0);

			wait(NULL);

			skdInfo->localPid = getpid();
			pStateSwitch(0, i);
			clockInc(skdInfo->burstTime);
			// logger(progName, "Returned to OSS ", getpid());
			snprintf(buf, BUF_SIZE, "Process %i Burst Time: %i", i, skdInfo->burstTime);
			logger(progName, buf, getpid());

			msgrcv(msqId, &msg, sizeof(msg), 1, 0);
			snprintf(buf, BUF_SIZE, "Message: %s", msg.text);
			// logger(progName, buf, getpid());
			// printf("Message:%s \n", msg.text);

			remProcess(i);
			clockInc(10);
			logger(progName, "Removed child process", getpid());



			// printSkd();
			// printBitVec();
		}
	}
	logger(progName, "Time limit reached ", getpid());
	printf("\nLines in Logfile: %i \n", logLines);

	printf("Total Number of Processes = %i \n", count);
	printf("Total Time = %u:%u \n", skdInfo->sClock, skdInfo->nsClock);


	/* Detach and deallocate all shared memory in use */
	if (skdInfo != (void *) -1) { // Detach and Deallocate skdInfo
			// Detach
			memDtDl = shmdt(skdInfo);
			if (memDtDl != 0) { perror("skdInfo - shmdt"); }
			printf("\nskdInfo Detached . . . ");
			memDtDl = 0;
			// Deallocate
			memDtDl = shmctl(skdId, IPC_RMID, NULL);
			printf("Deallocated \n");
			if (memDtDl != 0) { perror("skdInfo - shmctl"); }

			memDtDl = 0;
		}
	if (pTable != (void *) -1) { // Detach and Deallocate pTable
			// Detach
			memDtDl = shmdt(pTable);
			if (memDtDl != 0) { perror("pTable - shmdt"); }
			printf(" pTable Detached . . . ");
			memDtDl = 0;
			// Deallocate
			memDtDl = shmctl(ptId, IPC_RMID, NULL);
			if (memDtDl != 0) { perror("pTable - shmctl"); }
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
