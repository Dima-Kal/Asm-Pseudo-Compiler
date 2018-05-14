/*
Input reader file = input_reader.c

 |=======================================================|
  |						 	|
  |	   By Dima Kailinichenko, (Id) 319606166	|
  |	     & Elad Ben-David, (Id) 300235066	 	|
  |							|
 |=======================================================|


*/

/*Include standard libraries and custom header*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

/*Global variable for the line number and database table*/
parsedLine *pl;
int line;
int errCnt = 0;
int sucCnt = 0;
int iLine = 1;
int yLine = 0;	
int jLine = 0;
/*The function gets input from file, allocates memory and sorts them in a structure.*/
int buildParsedLine(FILE *file) {
	/*Variables*/
	extern char* fileName;
	char fileExt[FILENAME_MAX];
	char fileEnt[FILENAME_MAX];
	char s;
	int counter = 1;
	int state = LABEL;
	int x = 0;
	int pass = TRUE;
	char temp[] = "abcdef";
	FILE *ent;
	FILE *ext;
	strcpy(fileEnt, fileName);
	strcpy(fileExt, fileName);
	strcat(fileEnt, ".en");
	strcat(fileExt, ".ex");
	/*Overwrite old files*/
	ent = fopen(fileEnt, "w");
	ext = fopen(fileExt, "w");
	fclose(ent);
	fclose(ext);
	/*Use Line Counter to know how many lines are in the input file*/
	line = lineCounter(file);
	/*Set file pointer to start of file*/
	fseek(file, SEEK_SET, 0);
	s = fgetc(file);
	/*Allocate memory for the parsed line according to the files.*/
	pl = calloc(line, sizeof(parsedLine));
	if (!pl) {
		error(ERR_MEMORY_ALLOCATION);
		exit(0);
	}
	/*Loop through the lines and sort the data into the struct*/
	while (iLine <= line) {
		pl[iLine].inputLine = counter;
		counter++;
		state = LABEL;
		/*If comment skip line*/

		if (s == ';') {
			while (s != '\n') {
				s = fgetc(file);
			}
			yLine++;
			line--;
		}
		/*Checks if there is any data that needs to go to external files and outputs it*/
		else if (s == '.') {
			yLine++;
			line--;
			x = 0;
			/*Run until the end of the word and check validality */
			while (x < 6 && s != ' ' && s != '\t' && s != '\n' && s != EOF) {
				s = fgetc(file);
				temp[x] = s;
				x++;
			}
			/*Checks if entry*/
			if (strcmp("entry ", temp) == 0) {
				FILE *ent = fopen(fileEnt, "a");
				while (s != '\n' && s != EOF) {
					s = getc(file);
					fputc(s, ent);
				}
				fputc(s, ent);
				fclose(ent);
			}
			/*Checks if external*/
			else if (strcmp("extern", temp) == 0) {
				FILE *ext = fopen(fileExt, "a");
				while (s != '\n' && s != EOF) {
					s = getc(file);
					fputc(s, ext);
				}
				fputc(s, ext);
				fclose(ext);
			}
			/*If neither, skip the row and print error*/
			else {
				pass = FALSE;
				error(ERR_WRONG_INPUT);
				while (s != '\n' && s != EOF)
					s = getc(file);
			}
		}
		/*Now for the rest of the input*/
		else {
			/*If the line is not a label, we skip spaces*/
			if (isSpace(s)) {
				while (isSpace(s)) {
					s = fgetc(file);
				}
				if (s == ';') {
					while (s != '\n')
						s = fgetc(file);
				}
				if (s == '\n' || s == EOF) {
					line--;
					yLine++;
					iLine--;
				} else
					state = COMMAND;
			}
			/*Insert elements into the structure by states*/
			while (s != '\n' && s != EOF) {
				if (state == LABEL) {
					if (jLine == MAX_INPUT && s != ':') {
						pass = FALSE;
						error(ERR_MEMORY_MAX_LABEL);
						state = COMMAND;
						jLine = 0;
					} else if (s != ':') {
						if (jLine == 0 && isLetter(s) == 0) {
							pass = FALSE;
							error(ERR_LABEL_CHARACTER);
							pl[iLine].label[jLine] = s;
							jLine++;
						} else if (isLetter(s) == 0 && isDigit(s) == 0
								&& jLine != 0) {
							pass = FALSE;
							error(ERR_WRONG_LABEL);
							pl[iLine].label[jLine] = s;
							jLine++;
						}
						pl[iLine].label[jLine] = s;
						jLine++;
					}
					/*Done managing labels*/
					/*Managing commands*/
					else {
						state = COMMAND;
						jLine = 0;
					}
				} else if (state == COMMAND) {
					if (jLine == MAX_INPUT && s != ' ' && s != '\t') {
						pass = FALSE;
						error(ERR_MEMORY_COMMAND);
						state = OPERAND1;
						jLine = 0;
					}
					/*managing Operands*/
					else if (jLine == 0 && s != ' ' && s != '\t') {
						pl[iLine].command[jLine] = s;
						jLine++;
					} else if (jLine > 0 && s != ' ' && s != '\t') {
						pl[iLine].command[jLine] = s;
						jLine++;
					} else if ((jLine > 0 && s == ' ') || (jLine > 0 && s == '\t')) {
						state = OPERAND1;
						jLine = 0;
					}
				} else if (state == OPERAND1) {
					if (jLine == MAX_INPUT && s != ' ' && s != '\t' && s != ',') {
						pass = FALSE;
						error(ERR_MEMORY_OPERAND1);
						state = OPERAND2;
						jLine = 0;
					} else if (jLine >= 0 && s != ' ' && s != '\t' && s != ',') {
						pl[iLine].operand1[jLine] = s;
						jLine++;
					} else if ((jLine > 0 && s == ' ') || (jLine > 0 && s == '\t')
							|| (jLine > 0 && s == ',')) {
						state = OPERAND2;
						jLine = 0;
					}
				} else if (state == OPERAND2) {
					if (jLine == MAX_INPUT && s != ' ' && s != '\t') {
						pass = FALSE;
						error(ERR_MEMORY_OPERAND2);
						exit(0);
					} else if (jLine < MAX_INPUT && s != ' ' && s != '\t') {
						pl[iLine].operand2[jLine] = s;
						jLine++;
					}
				}
				s = fgetc(file);
			}
			iLine++;
		}
		jLine = 0;

		s = fgetc(file);
	}
	/*Check if we have any ext or ent that insert to command cause the line doesn't start with a dot - and fix it*/
	iLine = 0;
	while (iLine <= line) {
		x = 0;
		strcpy(temp, pl[iLine].command);
		if (strcmp(".entry", temp) == 0) {
			FILE *ent = fopen(fileEnt, "a");
			fputs(pl[iLine].operand1, ent);
			fclose(ent);
			x = 1;
		} else if (strcmp(".extern", temp) == 0) {
			FILE *ext = fopen(fileEnt, "a");
			fputs(pl[iLine].operand1, ext);
			fclose(ext);
			x = 1;
		}
		if (x != 0) {
			jLine = iLine;
			while (jLine < line) {
				strcpy(pl[jLine].label, pl[jLine + 1].label);
				strcpy(pl[jLine].command, pl[jLine + 1].command);
				strcpy(pl[jLine].operand1, pl[jLine + 1].operand1);
				strcpy(pl[jLine].operand2, pl[jLine + 1].operand2);
				pl[jLine].inputLine = pl[jLine + 1].inputLine;
				jLine++;
			}
			line--;
			yLine++;
		}
		iLine++;
	}

	return pass;

}

int isDynamic(char operand[], int i) {
	char delims[5] = "[ - ]";
	char* sPtr;
	char* newOp;
	int rangeStart;
	int rangeEnd;

	sPtr = (char *) malloc(MAX_INPUT);
	strcpy(sPtr, operand);

	sPtr = strtok(sPtr, delims);
	newOp = (char *) malloc(MAX_INPUT);
	strcpy(newOp, sPtr);

	if (!isLetter(sPtr[0]))
		return FALSE;
	else {
		setData(i, OPERAND1, newOp);
	}

	sPtr = strtok(NULL, delims);

	if (sPtr != NULL) {

		if (!isDigit(sPtr[0]))
			return FALSE;
		else
			rangeStart = atoi(sPtr);
	}

	sPtr = strtok(NULL, delims);

	if (sPtr != NULL) {
		if (!isDigit(sPtr[0]))
			return FALSE;
		else
			rangeEnd = atoi(sPtr);
	}

	sPtr = strtok(NULL, delims);
	if (sPtr != NULL) {
		error(ERR_WRONG_OPERAND_CHARACTER);
	}

	pl[i].dynamicRange = rangeEnd - rangeStart;
	pl[i].dynStart = rangeStart;

	return TRUE;
}

/*Gets data for a single parameter and index line*/
char* getData(int index, int param) {
	if (index > line || index < 1) {
		error(ERR_INDEX_OUT_OF_BOUND);
		return '\0';
	}
	if (param == LABEL)
		return pl[index].label;
	else if (param == COMMAND)
		return pl[index].command;
	else if (param == OPERAND1)
		return pl[index].operand1;
	else if (param == OPERAND2)
		return pl[index].operand2;
	else {
		error(ERR_WRONG_PARAMETER);
		return '\0';
	}
}

/*retun dynamic range*/
int getDynamic(int index) {
	return pl[index].dynamicRange;
}

/*return dynamic start range*/
int getDynamicStart(int index) {
	return pl[index].dynStart;
}

/*Gets the line number in the input file*/
int getInputLine(int i) {
	if (i > line || i < 0)
		return 0;
	else
		return pl[i].inputLine;
}

/*Sets data parameter on our parsed line */
void setData(int index, int param, char data[]) {
	if (index <= line || index >= 1) {
		if (param == LABEL)
			strcpy(pl[index].label, data);
		else if (param == COMMAND)
			strcpy(pl[index].command, data);
		else if (param == OPERAND1)
			strcpy(pl[index].operand1, data);
		else if (param == OPERAND2)
			strcpy(pl[index].operand2, data);
	} else {
		error(ERR_INDEX_OUT_OF_BOUND);
	}
}
/*Count lines of data for the memory allocation*/
int lineCounter(FILE *file) {
	int counter = 0;
	char c;
	/*Going back to beginning of the file*/
	fseek(file, SEEK_SET, 0);
	/*Check if have 1 line at least */
	if (fgetc(file) != EOF)
		counter++;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n')
			counter++;
	}
	/*return number lines */
	return counter;
}

/*Free memory of the global mat*/
void freeParsedMemory() {
	free(pl);
}

