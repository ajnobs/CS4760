// Name: Aj Nobs
// Date: 03/22/21
// Course: CS 4760
// Project: Assignment 4
// File Name: user.c
// Project Files: oss.c oss.h user.c user.h utility.c utility.h Makefile README VersionControl
// Description: Simulates an operating system scheduler. Randomly creates a child process and uses
//				a message queue alongside shared memory to keep the processes in sync. Logging is done
//				along with tracking of system resource such as time and current process running.
//-----------------------------------------------------------------------------------------------------

#include "user.h"

int main(int argc, char *argv[]) {
	progName = argv[1];
	logName = argv[2];
	lFile = fopen(logName, "w+");
	if (lFile == NULL) { errorExit(progName, "fopen"); }

	char buf[BUF_SIZE];

	scheduleShm();
	processTableShm();
	createMessageQueue();

	msgrcv(msqId, &msg, sizeof(msg), 1, 0);
	snprintf(buf, BUF_SIZE, "Message: %s", msg.text);
	// logger(progName, buf, getpid());

	// logger(progName, "User process start ", getpid());
	// printBitVec();

	int n;
	time_t ti; // Rand seed
	srand((unsigned) time(&ti)); // Rand initialization
	skdInfo->localPid = getpid();
	skdInfo->burstTime += 100;

	// printSkd();

	msg.text[0] = 'd';
	msgsnd(msqId, &msg, sizeof(msg), 0);

	n = rand() % 99 + 1;
	skdInfo->burstTime += n;

	// logger(progName, "User process return ", getpid());
	return 0;
}