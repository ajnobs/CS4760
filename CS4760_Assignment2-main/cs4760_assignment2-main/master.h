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


#ifndef CS4760_ASSIGNMENT2_MASTER_H
#define CS4760_ASSIGNMENT2_MASTER_H

void detachdealloc(); // Detach and deallocate function

void sighandler(int signum); // Signal handler function

int main(int argc, char *argv[]); // Main function

#endif //CS4760_ASSIGNMENT2_MASTER_H
