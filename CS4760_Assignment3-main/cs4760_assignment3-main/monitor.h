// Name: Aj Nobs
// Date: 03/06/21
// Course: CS 4760
// Project: Assignment 3
// File Name: monitor.h
// Project Files: monitor.c monitor.h statMon.c statMon.h consumer.c consumer.h producer.c producer.h Makefile README
// Description: 
//				      
//              
//              
//-----------------------------------------------------------------------------------------------------

#ifndef CS4760_ASSIGNMENT3_MONITOR_H
#define CS4760_ASSIGNMENT3_MONITOR_H

int pNum = 0;
int cNum = 0;

static void handler(int signal);
static void timer(int seconds);
static void exitSafe();
static void newChild(int childNum);

int main(int argc, char *argv[]); // Main function


#endif //CS4760_ASSIGNMENT3_MONITOR_H
