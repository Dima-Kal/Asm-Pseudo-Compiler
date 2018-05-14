/*
Main file = main.c

 |=======================================================|
  |						 	|
  |	   By Dima Kailinichenko, (Id) 319606166	|
  |	     & Elad Ben-David, (Id) 300235066	 	|
  |							|
 |=======================================================|


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "assembler.h"

extern int errCnt;
extern int sucCnt;

char* fileName;
/*Delete temp file*/
void removeTempFiles(char* fileName1) {

	char temp[FILENAME_MAX];
	strcpy(temp, fileName1);
	strcat(temp, ".ex");
	remove(temp);
	strcpy(temp, fileName1);
	strcat(temp, ".en");
	remove(temp);
}

int main(int argc, char* argv[]) {
	char inputFile[FILENAME_MAX];
	int pass = TRUE;
	int i;
	FILE *file;
	time_t t;
	if (argc == 1) {
		error(ERR_NO_FILE);
		return 0;
	}

	srand((unsigned) time(&t));

	/*Loop through file names.*/
	for (i = 1; i < argc; i++) {
		fileName = argv[i];
		strcpy(inputFile, argv[i]);
		strcat(inputFile, ".as");

		file = fopen(inputFile, "r");
		if (!file) {
			error(ERR_FILE_NOT_EXIST);
			return 0;
		}
		/*Collect the input from the file and make first validation*/
		pass = buildParsedLine(file);
		/*Validate commands and labels*/
		if (!validateLabels())
			pass = FALSE;
		if (!validateAllCommands())
			pass = FALSE;

		/*If flag is TRUE, can go on and parser the input*/
		if (pass) {
			createSymbolTable();
			createOutputTable();
			freeTables();
			/*Delete temp files*/
			removeTempFiles(fileName);
			success(SUCC_COMPILATION);
		} else {
			/*Print error*/
			error(ERR_COMPILATION_FAILED);
			/*Delete temp file*/
			removeTempFiles(fileName);
		}
		/*Free input table*/
		freeParsedMemory();
		fclose(file);
	} /*end for*/

	return 0;
}

