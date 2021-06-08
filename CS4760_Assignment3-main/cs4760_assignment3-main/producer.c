// Name: Aj Nobs
// Date: 03/06/21
// Course: CS 4760
// Project: Assignment 3
// File Name: producer.c
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
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "statMon.h"
#include "producer.h"

static int pos;

int main(int argc, char *argv[]) {
	printf("\nProducer exec ");

	sigAct(SIGTERM, handler); // Signal from Ctrl+C
	sigAct(SIGUSR1, handler); // Signal from time out







	return 0;
}

static void handler(int signal) {
	semWait(2);

	char msg[1024];

	sprintf(msg, "Process %d exiting, Signal: %s \n", pos, signal == SIGUSR1 ? "timeout" : "interrupt");
	fprintf(stderr, msg);

	logger("output.log", msg);

	semSignal(2);

	exit(EXIT_FAILURE);
}
