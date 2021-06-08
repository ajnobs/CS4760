// Name: Aj Nobs
// Date: 02/19/21
// Course: CS 4760
// Project: Assignment 2
// File Name: master.c
// Project Files: master.c master.h bin_adder.c bin_adder.h Makefile README a2df.txt
// Description: This program uses fork and exec to create a binary tree algorithm for summing up values
//				provided by the data file.  The user can set maximum number of processes or execution
//				time using the -s or -t options.  The -h prints a help screen.
//-----------------------------------------------------------------------------------------------------

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include "master.h"

char *msg; // Custom perror() message
int shmid;
void *addr;
static int dd = 0; sig = 0;

int main(int argc, char *argv[]) {
	clock_t start_t, end_t, total_t; // Timer variables
	FILE *data, *adder_log; // Files
	int opt, n = 1, s = 20, t = 100; // Command line variables
	int sFlag = 0, tFlag = 0; // Option flags
	int status, pid, pNum = 0, idx = 0, dLvl = 0, depth = 0; // Process variables
	msg = argv[0]; // Custom perror() message
	char c; // Generic character variable
	char *str = NULL; // Generic string variable
	int num = 0, i, j, k; // Generic integer variables
	int  adj = 1, t1 = 0, t2 = 0; // Fork variables


	if (argc == 1) { // Catches incorret number of arguments
		printf("\nInvocation must include a data file \n\n");
		exit(EXIT_FAILURE);
	} else if (argc == 3 || argc ==5 || argc > 7) {
		printf("\nIncorrect number of arguments.  Must have data file and may inlude the options -s and/or -t \n\n");
		exit(EXIT_FAILURE);
	}


	// Get command line arguments
	while((opt = getopt(argc, argv, ":hs:t:")) != -1) { // Loop over argv looking for option args
		switch (opt) {

			case 'h': // Print doenv utility help page
				printf("\nNAME \n     master - gets sum of integers in provided file ");
				printf("\nUSAGE \n     master [OPTION] ... DATAFILE ");
				printf("\nDESCRIPTION \n     Uses a binary tree algorithm to create child processes that sum the integers provided by the data file.");
				printf("\nOPTIONS \n     -h, help \n              display this help and exit ");
				printf("\n     -s i, max children \n              set maximum number of allowed children at any given time, default value 20 ");
				printf("\n     -t time, termination time \n              set the time in seconds before the program terminates (regardless of current status), default value 100 ");
				// printf("\n     If no COMMAND is provided, the environment will be printed ");
				printf("\nAUTHOR ");
				printf("\n     Written by Aj Nobs\n\n");
				exit(EXIT_SUCCESS);

			case 's': // Set simultaneous children maximum limit
				sFlag = 1;
				printf("\noptarg = %s ", optarg);
				s = strtol(optarg, NULL, 10);
				if (s == 0) {
					msg = argv[0];
					strcat(msg, ": Entered value of s is invalid, please enter an integer between 2 and 10 (inclusive) ");
					printf("\n");
					perror(msg);
					exit(EXIT_FAILURE);
				}
				if (s > 20) {
					s = 20;
					printf("\n20 is the maximum allowed processes ");
				} else if (s < 2) {
					s = 20;
					printf("\n2 is the minimum allowed processes ");
				}
				break;

			case 't': // Set time until termination
				tFlag = 1;
				printf("\noptarg = %s ", optarg);
				t = strtol(optarg, NULL, 10);
				if (t == 0) {
					msg = argv[0];
					strcat(msg, ": Entered value of t is invalid, please enter an integer of 1 or greater ");
					printf("\n");
					perror(msg);
					exit(EXIT_FAILURE);
				}
				if (t <= 0) {
					t = 100;
					printf("\n1 is the minimum allowed time for execution ");
				}
				break;

			case '?': // Unrecognized option
				msg = argv[0];
				strcat(msg, ": Error: Unrecognized option ");
				strncat(msg, (const char *) optopt, 1);
				printf("\n");
				perror(msg);
				break; //return -1;

			default: // Unexpected exit
				msg = argv[0];
				strcat(msg, ": Error: Unexpected issue during parsing ");
				printf("\n");
				perror(msg);
				exit(EXIT_FAILURE); //return -1;
		}
	}


	// Set defaults based on flags
	if (sFlag == 0) { s = 20; }
	printf("\ns: %d [%d] ", s, sFlag);
	if (tFlag == 0) { t = 100; }
	printf("\nt: %d [%d] ", t, tFlag);


	// Open datafile and count number of values
	data = fopen(argv[optind], "r");
	if (data == NULL) { // File error print
		msg = argv[0];
		strcat(msg, ": Failed to open provided file ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nData file successfully opened ");
	}
	while ((c = fgetc(data)) != EOF) {
		if (c == '\n') {
			n++;
		}
	}
	num = n;
	while (num > 1) { // Find required depth
		num = num / 2;
		dLvl++;
	}
	printf("\nn = %d,  dLvl = %d", n, dLvl);


	// Timer
	start_t = clock();
	// printf("\n\nStart: %lu ", start_t);


	// Allocate and deallocate shared memory
	key_t key = ftok("master.c", 1);; // Create main key
	if (key == (key_t) -1) {
		msg = argv[0];
		strcat(msg, ": Failed create key ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nKey created [%lld]", key);
	}
	shmid = shmget(key, ((n+10) * sizeof(int)), IPC_CREAT | 0666); // Create shared memory id
	if (shmid == -1) {
		msg = argv[0];
		strcat(msg, ": Failed allocate shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory created [%d] ", shmid);
	}

	addr = (void *) shmat(shmid, NULL, 0); // Create address of shared memory
	if (addr == (int *) -1) {
		msg = argv[0];
		strcat(msg, ": Failed attach shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory attached [%d]", addr);
	}


	// Create and fill array in shared memory with values read from data file
	int *arr = addr;
	rewind(data);
	for (i=0; i<n; i++) {
		fscanf(data, "%d", &num);
		arr[i] = num;
	}
	fclose(data); // Close data file


	// Signal monitoring start
	signal(SIGINT, sighandler);


	// Create logfile "output.log"
	adder_log = fopen("output.log", "w+");
	if (adder_log == NULL) {
		msg = argv[0];
		strcat(msg, ": Failed to create and open file ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nOutput.log successfully created and opened ");
	}
	fprintf(adder_log, "%s", "Time     PID      Index     Depth \n"); // Top line of log file


	// Create binary tree of processes
	char aStr[sizeof(char *)];
	sprintf(aStr, "%d", n);
	char *args[] = {".bin_adder", aStr, NULL};
	arr[n+1] = clock();
	// printf("\narr[n+1]: %d ", arr[n+1]);
	pid = fork();
	pNum++;

	if (pid == 0) {
		printf("\n\n");
		start_t = clock() + arr[n+1];
		printf("\nCurrent Clock: %lu", start_t);
		printf("\narr[0] = %d ", arr[0]);

		/*
		while (i < 100000000000000) { // Basically endless loop
			i++;
		}
		 */

		adj = 1;
		for (i = 0; i < dLvl; i++) { // Level loop
			arr[n+7] = i; // Level
			for (k = 0; k < i; k++) { // Number of values
				num = num * 2;
			}
			pid = fork();
			wait(&status);

			adj = (i+1) * 2;
			for (j = 0; j < (num / 2); j+=2) {
				t1 = j * adj;
				t2 = t1 + (adj / 2);
				arr[n+8] = t1;
				arr[n+9] = t2;
				printf("\narr[%d] = %d,  ", t1, arr[t1]);
				printf("arr[%d] = %d", t1, arr[t1]);
				end_t = clock();
				// printf("\nEnd: %lu ", end_t);
				arr[n+1] = end_t;
				// printf("\narr[n+1]: %d ", arr[n+1]);
				pid = fork();
				wait(&status);

			}
			wait(&status);
			execvp("./bin_adder", args);
		}

		/*
		printf("\nFinished execvp \n");
		end_t = clock();
		arr[n+1] += end_t;
		exit(0);
		 */
	}
	wait(&status);

	end_t = arr[n+1];
	// printf("\nEnd: %lu ", end_t);
	total_t = (end_t - start_t);
	printf("\nTotal: %lu ", (end_t-start_t));
	double timer = (double) total_t / CLOCKS_PER_SEC; // Total in seconds
	printf("\nTotal Time: %f sec \n", timer);

	for (i=0; i < n; i++) {
		printf("\narr[%d] = %d ", i, arr[i]);
	}
	printf("\n");


	// Detach and deallocate shared memory
	detachdealloc (addr, shmid, argv[0]);
	/*
	num = shmdt(addr);
	if (num == -1) {
		msg = argv[0];
		strcat(msg, ": Failed detach shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory detached [%d]", num);
	}
	num = shmctl(shmid, IPC_RMID, NULL);
	if (num == -1) {
		msg = argv[0];
		strcat(msg, ": Failed deallocate shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory deallocated [%d]", num);
	}
	 */
	fclose(adder_log); // Close log file

	printf("\n\n");
	return 0;
}

void detachdealloc() {
	dd++;
	if (dd <= 1 && sig <= 1) {
		int num = 0;

		num = shmdt(addr);
		if (num == -1) {
			strcat(msg, ": Failed detach shared memory ");
			printf("\n");
			perror(msg);
			exit(EXIT_FAILURE);
		} else {
			printf("\nShared memory detached [%d]", num);
		}

		num = shmctl(shmid, IPC_RMID, NULL);
		if (num == -1) {
			strcat(msg, ": Failed deallocate shared memory ");
			printf("\n");
			perror(msg);
			exit(EXIT_FAILURE);
		} else {
			printf("\nShared memory deallocated [%d]", num);
		}
		printf("\n");
	}
}

void sighandler(int signum) { // Signal handler function
	sig++;
	if (sig <= 1 && dd <= 1) {
		printf("\nInterrupt signal detected, Cleaning memory... \n"); // Interrupt signal print

		detachdealloc();

		kill(getpid(), SIGINT);

		abort();
	}
}
