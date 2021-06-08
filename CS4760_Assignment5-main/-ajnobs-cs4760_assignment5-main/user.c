// Name: Aj Nobs
// Date: 04/20/21
// Course: CS 4760
// Project: Assignment 5
// File Name: user.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Implementation of a resource management module for our Operating System Simulator OSS.
//-----------------------------------------------------------------------------------------------------


#include "user.h"

int main(int argc, char *argv[]) {
	progName = argv[1];
	logName = argv[2];
	lFile = fopen(logName, "w+");
	if (lFile == NULL) { errorExit(progName, "fopen"); }

	char buf[BUF_SIZE];

	clockShm();
	resourceShm();
	createMessageQueue();

	msgrcv(msqId, &msg, sizeof(msg), 1, 0);
	snprintf(buf, BUF_SIZE, "Message: %s", msg.text);
	// logger(progName, buf, getpid());

	// logger(progName, "User process start ", getpid());
	// printBitVec();

	int n;
	time_t ti; // Rand seed
	srand((unsigned) time(&ti)); // Rand initialization
	SHCLOCK->localPid = getpid();
	clck->burstTime += 100;

	// printSkd();

	msg.text[0] = 'd';
	msgsnd(msqId, &msg, sizeof(msg), 0);

	n = rand() % 99 + 1;
	clck->burstTime += n;

	// logger(progName, "User process return ", getpid());
	return 0;
}