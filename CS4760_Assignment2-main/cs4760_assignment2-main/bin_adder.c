// Name: Aj Nobs
// Date: 02/19/21
// Course: CS 4760
// Project: Assignment 2
// File Name: bin_adder.c
// Project Files: master.c master.h bin_adder.c bin_adder.h Makefile README a2df.txt
// Description: This program uses fork and exec to create a binary tree algorithm for summing up values
//				provided by the data file.  The user can set maximum number of processes or execution
//				time using the -s or -t options.  The -h prints a help screen.
//-----------------------------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include "bin_adder.h"

int main(int argc, char *argv[]) {
	printf("\nChild created and entered ");
	char *msg = argv[0];
	int num = 0, n = 0;
	n = strtol(argv[1], NULL, 10);
	printf("\nArgv[1] = %s ", argv[1]);
	// printf("\nn = %d ", n);

	// Shared memory tasks
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
	int shmid = shmget(key, ((n+10) * sizeof(int)), IPC_CREAT | 0666); // Create shared memory id
	if (shmid == -1) {
		msg = argv[0];
		strcat(msg, ": Failed allocate shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory created [%d] ", shmid);
	}
	void *addr = (void *) shmat(shmid, NULL, 0); // Create address of shared memory
	if (addr == (int *) -1) {
		msg = argv[0];
		strcat(msg, ": Failed attach shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory attached [%d] ", addr);
	}
	int *arr = addr;

	clock_t start_t = arr[n+1];


	// Sum math with arr[n+8] and arr[n+9] being array index of summed values
	int a1 = arr[n+8], a2 = arr[n+9];
	arr[a1] = arr[a1] + arr[a2];
	// printf("\narr[%d] = %d", a1, arr[a1]);
	arr[a2] = 0;


	clock_t end_t = clock();
	arr[n+1] = end_t;


	// Detach shared memory
	num = shmdt(addr);
	if (num == -1) {
		msg = argv[0];
		strcat(msg, ": Failed detach shared memory ");
		printf("\n");
		perror(msg);
		exit(EXIT_FAILURE);
	} else {
		printf("\nShared memory detached [%d] ", num);
	}

	printf("\n\n");
	return 0;
}