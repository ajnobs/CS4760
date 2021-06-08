// Name: Aj Nobs
// Date: 03/06/21
// Course: CS 4760
// Project: Assignment 3
// File Name: monitor.c
// Project Files: monitor.c monitor.h statMon.c statMon.h consumer.c consumer.h producer.c producer.h Makefile README
// Description:
//
//
//
//-----------------------------------------------------------------------------------------------------
#include <sys/shm.h>
#include <ctype.h> //
#include <getopt.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h> //
#include <stdbool.h> //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h> //
#include <sys/time.h> //
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "statMon.h"
#include "monitor.h"


int main(int argc, char *argv[]) {
	resetBuf(argc, argv); // Reset output buffers
	int opt; // CL option variable
	int status; // Child status variable
	int i, j, k; // Generic variables


	// Get command line argument options
	while(true) {
		opt = getopt(argc, argv, "ho:p:c:t:");
		if (opt == -1) { break; } // Exit loop

		switch (opt) {

			case 'h': // Print monitor help page
				printf("\nNAME \n     monitor - run the producer-consumer problem ");
				printf("\nUSAGE \n     monitor [OPTION]  ");
				printf("\nDESCRIPTION \n     Uses a main monitor program, semaphores, and two types of child processes to run the producer consumer-problem.");
				printf("\nOPTIONS \n     -h, help \n              display this help and exit ");
				printf("\n     -o logfile, name of logfile \n              set the name of the logfile to be used, default: logfile ");
				printf("\n     -p m, number of producers \n              set the number of producers, default: m=2 ");
				printf("\n     -c n, number of consumers \n              set the number of consumers, default: n=6 ");
				printf("\n     -t time, time until termination \n        set the time in seconds before the program terminates (regardless of current status), default: t=100 ");
				printf("\nAUTHOR ");
				printf("\n     Written by Aj Nobs\n\n");

				exit(EXIT_SUCCESS);

			case 'o': // Set logfile name
				if ((logName = optarg) == NULL) {
					perror("Failed to create logfile");
					exit(EXIT_FAILURE);
				}
				break;

			case 'p': // Set producer
				if (!isdigit(*optarg)) { // Number check m
					perror("Invalid m value (Non-Number)");
					exit(EXIT_FAILURE);
				}
				m = atoi(optarg); // Assign m
				if (m <= 0) { // Range check m
					perror("Invalid m value (Less than 0)");
					exit(EXIT_FAILURE);
				}
				break;

			case 'c': // Set consumer
				if (!isdigit(*optarg)) { // Number check n
					perror("Invalid n value (Non-Number)");
					exit(EXIT_FAILURE);
				}
				n = atoi(optarg); // Assign n
				if (n <= 0) { // Range check n
					perror("Invalid n value (Less than 0)");
					exit(EXIT_FAILURE);
				}
				break;

			case 't': // Set time until termination
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

			default: // Unexpected exit
				perror("Unexpected error during parsing options");
				exit(EXIT_FAILURE); // return -1;
		} // End of CL Option switch
	} // End of CL Option while loop

	// Open logfile
	if (logName == NULL) { logName = "logfile"; }
	// strcat(logName, ".log");
	logFile = fopen(logName, "w+");
	if (logFile == NULL) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "fopen");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}
	fclose(logFile);

	printf("\nInput values: o=%s  p=%d  c=%d  t=%d \n", logName, m, n, t); // Print input values

	sigAct(SIGINT, handler); // Start signal handler Ctrl+C signal

	timer(t); // Start timer

	memAlloc(1);
	memDealloc();

	semAlloc(1);
	semDealloc();


	for (i = 0; i <= (n+m); ++i) {
		printf("\nNew child %d", i);
		newChild(i);
	}

	wait(&status); // Wait for child to exit and store status





  return 0;
}

// Create new child process
static void newChild(int childNum) {
	pid_t pid = fork();

	printf("\npid=%d ", pid);
	if (pid == -1) { // Error & Exit
  		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "newChild - fork");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	} else if (pid == 0) { // Child
		if (childNum == 0) { setpidTemp(getpid()); }
		setpgid(0, getpidTemp()); // Set process PGID

		// Log message
		logger("logfile", "Process %d starting\n", childNum);

		// Execute consumer or producer
		char pos[1024];
		sprintf(pos, "%d", childNum);

		printf("\npNum=%d  cNum=%d ", pNum, cNum);

		if (pNum < m) { // Execute producer
			printf("\nproducer path ");
			pNum++;
			char *args[] = {".producer", NULL};
			execvp("./producer", args);
		}
		else if (cNum < n) { // Execute consumer
			printf("\nproducer path ");
			cNum++;
			char *args[] = {".consumer", NULL};
			execvp("./consumer", args);
		}


		exit(EXIT_SUCCESS);
	}
}

// Timer function and signal
static void timer(int seconds) {
	sigAct(SIGALRM, handler); // Timer signal

	struct itimerval itv; // Initialize timer

	itv.it_value.tv_sec = seconds;
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &itv, NULL) == -1) { // Error & Exit
		char buf[1024];

		snprintf(buf, 1024, "%s: %s", progName, "timer - setitimer");
		perror(buf);

		memDealloc();
		semDealloc();

		exit(EXIT_FAILURE);
	}
}

// Print and log exit status
static void exitSafe() {
	char msg[1024];

	// Create and print message
	sprintf(msg, "Exiting without error \n");
	fprintf(stderr, msg);

	// Log message
	logger("logfile", msg);

	// Release shared memory and semaphores
	memDealloc();
	semDealloc();
}


// Handles signals
static void handler(int signal) {
	char msg[1024];

	// Create and print message
	sprintf(msg, "Signal %s caught\n", signal == SIGALRM ? "timeout" : "interrupt");
	fprintf(stderr, msg);

	// Log message
	logger("logfile", msg);

	// Kill child processes
	killpg(getpidTemp(), signal == SIGALRM ? SIGUSR1 : SIGTERM);
	while (wait(NULL) > 0);

	exitSafe();

	exit(EXIT_SUCCESS);
}
