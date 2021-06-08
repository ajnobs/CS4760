// Name: Aj Nobs
// Date: 02/07/21
// Course: CS 4760
// Project: Assignment 1
// File Name: doenv.c
// Project Files: doenv.c doenv.h Makefile README
// Description: This program takes in arguments from the command line and acts in a similar way to the
// env utility.  Printing to stdout and adjust environ variables is the majority of its function
//-----------------------------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "doenv.h"
/* doenv [h]
 * doenv [-i] [var1=value] [var2=value] [...] {command1 [; command2] [; ...]}
 *
 * doenv: Error: Detailed error message */

extern char** environ; // Environment variable

int main(int argc, char *argv[]) {

	/* Generic counting variables i,j,k | Option arg variable | Argv index | Flag for -i option */
	int i, j, k, opt, index, iFlag = 0;
	char c; // Generic character variable
	char *temp = malloc(sizeof (char *)); // Generic string variable
	char *msg = argv[0]; // Custom perror() message
	char *str = malloc(sizeof (char *)); // Non-option arg variable

	opterr = 0; // Prevents getopt() automatic error message printing

	if (argc == 1) { // Check to see if any arguments are provided, Prints environ if no args
		for (i = 0; environ[i] != NULL; i++) {
			printf("\n%s ", environ[i]);
		}
		exit(EXIT_SUCCESS);
	}

	while((opt = getopt(argc, argv, ":ih")) != -1) { // Loop over argv looking for option args
		switch (opt) {
			case 'i': // Clear environ by setting it to NULL
				iFlag = 1;
				environ = NULL;
				break;

			case 'h': // Print doenv utility help page
				printf("\nNAME \n     doenv - run a program in a modified environment ");
				printf("\nUSAGE \n     doenv [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...] ");
				printf("\nDESCRIPTION \n     Set each NAME to VALUE in the environment and run COMMAND.");
				printf("\nOPTIONS \n     -i, ignore-environment \n              start with an empty environment");
				printf("\n     -h, help \n              display this help and exit");
				printf("\n     If no COMMAND is provided, the environment will be printed ");
				printf("\nAUTHOR ");
				printf("\n     Written by Aj Nobs\n\n");
				exit(EXIT_SUCCESS);

			case '?': // Unrecognized option
				msg = argv[0];
				strcat(msg, ": Error: Unrecognized option ");
				c = (char) optopt;
				strncat(msg, &c, 1);
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

	/* Count the number of environ variable and commmand arguements | Return value | Length variable */
	int evNum = 0, cNum = 0, ret = 0;
	for (index = optind; index < argc; index++) {
		temp = argv[index];
		strcpy(str, temp);
		// printf("\nstr: %s %d", str, (int)strlen(str));
		if (strstr(str, "=") != NULL) { evNum++; }
		else { cNum++; }
	}
	// printf("\nevNum=%d  cNum=%d ", evNum, cNum);

	/* Create and allocate memory to string arrays for environ variables and commands as needed */
	char** evArr;
	if (evNum > 0) { evArr = malloc((evNum+1) * sizeof(char *)); }
	char** cArr;
	if (cNum > 0) { cArr = malloc(cNum * sizeof(char *)); }
	char* name = malloc(sizeof(char *)); // Holds variable name
	char* value = malloc(sizeof(char *)); // Holds variable value


	/* Fill evArr and cArr based on the number of each argument counted */
	j = 0, k = 0;
	for (index = optind; index < argc; index++) {
		// printf("\nstr: %s ", str);
		temp = argv[index];
		strcpy(str, temp);
		if (strstr(str, "=") != NULL) { // If NAME=VALUE
			memcpy(&evArr[j], &argv[index], strlen(argv[index]));
			// printf("\nvar: %s %d ", evArr[j], j);
			j++;
		}
		else { // If COMMAND
			cArr[k] = argv[index];
			// printf("\ncmd: %s %d", cArr[k], k);
			k++;
		}
	}


	/* If there are environ variables, Modify or set them */
	// if (environ == NULL) { printf("\nenviron == NULL "); }
	if (evNum > 0) {
		evArr[evNum] = NULL;
		if (iFlag == 1) { // environ == NULL
			// printf("\n-i Flag");
			environ = malloc((evNum+1) * sizeof(char *));
			for (i = 0; i < evNum; i++) {
				memcpy(&environ[i], &evArr[i], strlen(evArr[i]));
			}
			environ[evNum] = NULL;
		}
		else {  // Inherited environ
			ret = 0;
			for (i = 0; i < evNum; i++) {
				memcpy(name, evArr[i], strlen(evArr[i]));
				strtok_r(name, "=", &value);
				ret = setenv(name, value, 1);
				// printf("\nret= %d ", ret);
				if (ret != 0) {
					msg = argv[0];
					strcat(msg, ": Error: Failed to set environ variable");
					printf("\n");
					perror(msg);
					exit(EXIT_FAILURE); //return -1;
				}
			}
		}
	}


	/* If there are commands, Run those commands using system() */
	if (cNum > 0) {
		ret = 0;
		for (i = 0; i < cNum; i++) {
			temp = cArr[i];
			strcpy(str, temp);
			// printf("\ncmd: %s %d", str, i);
			ret = system(cArr[i]);
			// printf("\nret= %d ", ret);
			if (ret == -1) {
				msg = argv[0];
				strcat(msg, ": Error: Command failed");
				printf("\n");
				perror(msg);
				exit(EXIT_FAILURE); //return -1;
			}
		}
	}

	/* No commands provided but environ variables provided, print variables */
	if (cNum == 0 && evNum > 0) {
		for (i = 0; i < evNum; i++) { printf("\n%s ", evArr[i]); }
		printf("\n");
	}

	/* Free dynamic memory if allocated */
	if (evNum > 0) { free(evArr); }
	if (cNum > 0) { free(cArr); }

	printf("\nCOMPLETE\n\n");
	return 0;
}
