/*
Parser file = parser.c

 |=======================================================|
  |						 	|
  |	   By Dima Kailinichenko, (Id) 319606166	|
  |	     & Elad Ben-David, (Id) 300235066	 	|
  |							|
 |=======================================================|


 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "assembler.h"

extern int errCnt;
extern int sucCnt;

/*Global variables for the tables*/
symbolLine *symTable;
outputLine *outTable;

/*Length of all the symbols*/
int symbolLinesLength;
/*Length of the result output*/
int outLinesLength;

/*Function that creates the symbol table with the correct address*/
void createSymbolTable() {
	extern int line;
	extern char* fileName;
	char entFile[FILENAME_MAX];
	char temp[MAX_INPUT];
	int i;
	unsigned int x;
	int addressCounter = FIRST_ADDRESS;
	int dataAddress = 0;
	FILE *fent;
	/*Allocate memory*/
	symTable = calloc(line, sizeof(symbolLine));
	if (!symTable) {
		error(ERR_MEMORY_ALLOCATION);
		exit(0);
	}
	/*Handling the entry files*/
	strcpy(entFile, fileName);
	strcat(entFile, ".en");
	fent = fopen(entFile, "r");
	if (fent) {
		fclose(fent);
		strcat(entFile, "t");
		fent = fopen(entFile, "w+");
	}
	/*Moving operand1 to operand2 in case there is only single operand*/
	moveOper1to2();
	/*Running all over the input table taking the labels with the correct values*/
	for (i = 1, symbolLinesLength = 1; i <= line; i++) {
		if (isEmptyLine(i))
			continue;
		strcpy(temp, getData(i, LABEL));
		if (strlen(temp)) {
			strcpy(symTable[symbolLinesLength].symbol, temp);
			symTable[symbolLinesLength].address = addressCounter;
			if (!strcmp(DATALABEL, getData(i, COMMAND)) || !strcmp(STRINGLABEL, getData(i, COMMAND))) {
				symTable[symbolLinesLength].dataAdress = dataAddress;
				dataAddress += countLines(i);
			}
			symbolLinesLength++;
		}
		addressCounter += countLines(i);
	}
	for (i = 1, symbolLinesLength = 1; i <= line; i++) {
		if (isEmptyLine(i))
			continue;
		strcpy(temp, getData(i, LABEL));
		if (strlen(temp)) {
			if (!strcmp(DATALABEL, getData(i, COMMAND)) || !strcmp(STRINGLABEL, getData(i, COMMAND))) {
				symTable[symbolLinesLength].address = addressCounter
						- dataAddress + symTable[symbolLinesLength].dataAdress;
			}
			if (isEntry(temp)) {
				char temp8[MAX_SIZE_BASE8_NUM];
				x = (unsigned int) symTable[symbolLinesLength].address;
				intToBase8((unsigned int) x, temp8);
				fprintf(fent, "%s\t%s\n", temp, temp8);
			}
			symbolLinesLength++;
		}
	}
	/*The address counter should be the length of the output*/
	outLinesLength = addressCounter - FIRST_ADDRESS;
	fclose(fent);
	success(SUCC_CREATE_EN_FILE);
}
/*Creating the table with the output data*/
void createOutputTable() {
	/*variables*/
	extern int line;
	extern char* fileName;
	int i = 0, j = 0, x = 0, y = 0, len = 0, op1 = 0, op2 = 0, group = 0,
			opcode = 0, IC, DC, temp = 0;
	char IC8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
	char DC8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
	char operand1[MAX_INPUT];
	char operand2[MAX_INPUT];
	char command[MAX_INPUT];
	char oFile[FILENAME_MAX];
	char extFile[FILENAME_MAX];
	FILE *fp;
	FILE *fext;

	/*copying the file names and adding the correct extensions*/
	strcpy(oFile, fileName);
	strcpy(extFile, fileName);
	strcat(oFile, ".ob");
	strcat(extFile, ".ex");

	/*Opening the external file*/
	fext = fopen(extFile, "r");
	if (fext) {
		fclose(fext);
		strcat(extFile, "t");
		fext = fopen(extFile, "w+");
	}

	/*Allocate memory for the output*/
	outTable = calloc(outLinesLength, sizeof(outputLine));
	if (!outTable) {
		error(ERR_MEMORY_ALLOCATION);
		exit(0);
	}
	/*Inserting the code address to the table*/
	for (i = 0; i < outLinesLength; i++) {
		outTable[i].address = i + FIRST_ADDRESS;
	}

	/*inserting the code*/
	for (i = 1; i <= line; i++) {
		if (isEmptyLine(i))
			continue;
		/*Copying the input in the correct line*/
		memset(operand1, '\0', MAX_INPUT);
		memset(operand2, '\0', MAX_INPUT);
		memset(command, '\0', MAX_INPUT);
		strcpy(operand1, getData(i, OPERAND1));
		strcpy(operand2, getData(i, OPERAND2));
		strcpy(command, getData(i, COMMAND));
		if (command[0] != '.') {
			/*Find the correct code for the command*/
			if (strlen(getData(i, OPERAND2))) {
				if (strlen(getData(i, OPERAND1))) {
					group = GROUP1;
					if (!strcmp(command, "mov"))
						opcode = 0;
					else if (!strcmp(command, "cmp"))
						opcode = 1;
					else if (!strcmp(command, "add"))
						opcode = 2;
					else if (!strcmp(command, "sub"))
						opcode = 3;
					else if (!strcmp(command, "lea"))
						opcode = 6;
				} else {
					group = GROUP2;
					if (!strcmp(command, "not"))
						opcode = 4;
					else if (!strcmp(command, "clr"))
						opcode = 5;
					else if (!strcmp(command, "inc"))
						opcode = 7;
					else if (!strcmp(command, "dec"))
						opcode = 8;
					else if (!strcmp(command, "jmp"))
						opcode = 9;
					else if (!strcmp(command, "bne"))
						opcode = 10;
					else if (!strcmp(command, "red"))
						opcode = 11;
					else if (!strcmp(command, "prn"))
						opcode = 12;
					else if (!strcmp(command, "jsr"))
						opcode = 13;
				}
			}
			/*Handling the case of no operands*/
			else {
				group = GROUP3;
				if (!strcmp(command, "rts"))
					opcode = 14;
				else if (!strcmp(command, "stop"))
					opcode = 15;

			}
			/*Check status for the operands*/

			/*Calculate how many lines it will take in the object file*/
			len = countLines(i);

			/*operand1*/
			op1 = EMPTY;
			if (getDynamic(i) > 0) {
				op1 = DYNAMIC;
				outTable[i].dynRange = getDynamic(i);
				outTable[i].dynStart = getDynamicStart(i);
				strcpy(outTable[i].dynOP, operand1);
			}
			if (group == GROUP1 && isNumeric(operand1, i))
				op1 = NUMERIC;
			else if (group == GROUP1 && isLabel(operand1))
				op1 = LABEL;
			else if (group == GROUP1 && isRegister(operand1))
				op1 = REGISTER;

			/*operand2*/
			op2 = EMPTY;
			if (group != GROUP3 && isNumeric(operand2, i))
				op2 = NUMERIC;
			else if (group != GROUP3 && isLabel(operand2))
				op2 = LABEL;
			else if (group != GROUP3 && isRegister(operand2))
				op2 = REGISTER;

			/*Converting commands and data to object file*/
			for (j = 0; j < len; x++, j++) {
				/*Creating the command line*/
				if (j == FIRST_LINE) {
					temp = UNUSED_BITS;
					temp <<= TWO_BITS;
					temp += group;
					temp <<= FOUR_BITS;
					temp += opcode;
					temp <<= TWO_BITS;
					temp <<= TWO_BITS;
					temp += op2;
					temp <<= TWO_BITS;
					outTable[x].code = temp;
				}

				/*Code according to command groups*/
				else if (group == GROUP1 && j > FIRST_LINE) {
					/*In case of two registers, merging the numbers to one code line*/
					if (op1 == REGISTER && op2 == REGISTER) {
						temp = atoi(&operand1[1]);
						temp <<= SIX_BITS;
						temp += atoi(&operand2[1]);
						temp <<= TWO_BITS;
						outTable[x].code = temp;

					} else if (j == SECOND_LINE) {
						/*Creating the register code line*/
						if (op1 == REGISTER) {
							temp = atoi(&operand1[1]);
							temp <<= (SIX_BITS + TWO_BITS);
							outTable[x].code = temp;
						}
						/*Creating the number code line*/
						else if (op1 == NUMERIC) {
							temp = atoi(&operand1[1]);
							if (temp < 0)
								isNegative((unsigned int*) &temp);
							outTable[x].code = (unsigned int) temp;
							outTable[x].code <<= TWO_BITS;
						}
						/*Getting the label address from the symbol table*/
						else if (op1 == LABEL) {
							outTable[x].code = getSymbolAddress(operand1);
							outTable[x].code <<= TWO_BITS;
							outTable[x].code++;
							if (!isExtern(operand1))
								outTable[x].code++;
							else {
								char temp8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
								temp = outTable[x].address;
								intToBase8((unsigned int) temp, temp8);
								fprintf(fext, "%s\t\t\t%s\n", operand1, temp8);
							}
						}
					} else if (j == THIRD_LINE) {
						/*Creating the register code line*/
						if (op2 == REGISTER) {

							outTable[x].code = atoi(&operand2[1]);
							outTable[x].code <<= TWO_BITS;
						}
						/*creating the number code line*/
						else if (op2 == NUMERIC) {

							temp = atoi(&operand2[1]);
							if (temp < 0)
								isNegative((unsigned int*) &temp);

							outTable[x].code = (unsigned int) temp;
							outTable[x].code <<= TWO_BITS;
						}
						/*Getting the label address from the symbol table*/
						else if (op2 == LABEL) {
							outTable[x].code = getSymbolAddress(operand2);
							outTable[x].code <<= TWO_BITS;
							outTable[x].code++;
							if (!isExtern(operand2))
								outTable[x].code++;

							else {
								char temp8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8)
										+ 1] = "\0";
								temp = outTable[x].address;
								intToBase8((unsigned int) temp, temp8);
								fprintf(fext, "%s\t\t\t%s\n", operand2, temp8);
							}
						}
					}
				}

				else if (group == GROUP2 && j > FIRST_LINE) {
					/*Creating the register code line*/
					if (op2 == REGISTER) {

						outTable[x].code = atoi(&operand2[1]);
						outTable[x].code <<= TWO_BITS;
					}
					/*Creating the number code line*/
					if (op2 == NUMERIC) {
						temp = atoi(&operand2[1]);
						if (temp < 0)
							isNegative((unsigned int*) &temp);

						outTable[x].code = (unsigned int) temp;
						outTable[x].code <<= TWO_BITS;
					}
					/*Getting the label address from the symbol table*/
					if (op2 == LABEL) {

						outTable[x].code = getSymbolAddress(operand2);
						outTable[x].code <<= TWO_BITS;
						outTable[x].code++;
						if (!isExtern(operand2))
							outTable[x].code++;
						else {
							char temp8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] =
									"\0";
							temp = outTable[x].address;
							intToBase8((unsigned int) temp, temp8);
							fprintf(fext, "%s\t\t\t%s\n", operand2, temp8);
						}
					}
				}
			}
		}
	}

	/*Complete the output file by adding the data and string*/
	IC = outLinesLength - x;
	DC = x;
	intToBase8((unsigned int) DC, DC8);
	intToBase8((unsigned int) IC, IC8);
	for (i = 1; i <= line; i++) {
		strcpy(command, getData(i, COMMAND));
		if (command[0] == '.') {
			strcpy(operand1, getData(i, OPERAND1));
			strcpy(operand2, getData(i, OPERAND2));
			/*Calculate how many lines in the object file the string and the data required*/
			len = countLines(i);
			for (y = 0, j = 0; j < len; j++, x++) {
				/*String should get the ascii value for the letter*/
				if (!strcmp(command, STRINGLABEL)) {
					if (j < len - 1)
						outTable[x].code = operand2[j + 1];
					else
						outTable[x].code = 0;
				}
				/*Data should get the number*/
				else if (!strcmp(command, DATALABEL)) {
					if (j == 0) {
						temp = atoi(operand1);
						if (temp < 0)
							isNegativeOn15Bits((unsigned int*) &temp);
						outTable[x].code = (unsigned int) temp;
					} else {
						temp = atoi(&operand2[y]);
						if (temp < 0)
							isNegativeOn15Bits((unsigned int*) &temp);
						outTable[x].code = (unsigned int) temp;
						do {
							y++;
						} while (y < MAX_INPUT && operand2[y] != ','
								&& operand2[y] != '\0');

						if (operand2[y] == ',')
							y++;

					}
				}
			}
		}
	}
	/* Get the dynamic range value from info */

	for (i = 0; i < outLinesLength; i++) {
		if (outTable[i].dynOP[0] != '\0') {
			for (j = 0; j < symbolLinesLength; j++) {
				if (!strcmp(outTable[i].dynOP, symTable[j].symbol)) {
					x = symTable[j].address - 100;
					temp = outTable[x].code;
					temp = getBitRange(temp, outTable[i].dynStart,
							outTable[i].dynRange + outTable[i].dynStart);
					temp <<= TWO_BITS; /* add E,R,A Bits 00 */
					outTable[i].code = temp;

				}
			}
		}

	}

	/*Creating the object file*/
	fp = fopen(oFile, "w+");
	fprintf(fp, "\t%s %s", DC8, IC8);
	fputc('\n', fp);
	for (i = 0; i < outLinesLength; i++) {
		char address8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
		char code8[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
		intToBase8((unsigned int) outTable[i].address, address8);
		intToBase8Code((unsigned int) outTable[i].code, code8);
		fprintf(fp, "%s\t\t\t%s\n", address8, code8);
	}
	success(SUCC_CREATE_EX_FILE);
	success(SUCC_CREATE_OB_FILE);
	fclose(fp);
	fclose(fext);

}

/*The function returns the symbol address*/
int getSymbolAddress(char symbol[]) {
	int i;
	for (i = 0; i < symbolLinesLength; i++) {
		if (!strcmp(symbol, symTable[i].symbol))
			return symTable[i].address;
	}
	return 0;
}

/*Calculate how many code line the input line needed*/
int countLines(int i) {
	extern int line;
	int counter = 1;
	int j = 0;
	int len;
	char operand1[MAX_INPUT];
	char operand2[MAX_INPUT];
	char command[MAX_INPUT];
	strcpy(operand1, getData(i, OPERAND1));
	strcpy(operand2, getData(i, OPERAND2));
	strcpy(command, getData(i, COMMAND));
	if (i < 1 || i > line)
		return 0;
	if (!strcmp(command, DATALABEL)) {
		len = strlen(operand2);
		for (j = 0; j < len; j++) {
			if (isDigit(operand2[j])) {
				counter++;
				while (isDigit(operand2[j]) && j < len)
					j++;
			}
		}
		return counter;
	} else if (!strcmp(command, STRINGLABEL)) {
		len = strlen(operand2);
		/*the length minus the quotes*/
		return (counter + len - 2);
	} else {

		if (strlen(operand2)) {
			counter++;
			if (strlen(operand1)
					&& !(isRegister(operand1) && isRegister(operand2))) {
				counter++;
			}
		}
		return counter;
	}
}

void intToBase8Code(int num, char *buffer) {
	int i, j, c = -1;
	int tmp_num = num;
	char *digits = "!@#$%^&*";
	char bufferTmp[(MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1] = "\0";
	int codeSize = (MEM_WORD_SIZE) / (NUM_BITS_BASE8) + 1;

	/*special case num = 0*/
	if (!num) {
		bufferTmp[0] = digits[0];
	}

	/*Counting the number of digits required*/
	while (tmp_num) {
		c++;
		tmp_num /= BASE_NUM;
	}

	/*Storing the base8 in string*/
	while (c >= 0) {
		bufferTmp[c] = digits[num % BASE_NUM];
		num /= BASE_NUM;
		c--;
	}

	/*Placing ! inside unused cells*/
	for (i = 0; i < codeSize - strlen(bufferTmp) - 1; i++) {
		buffer[i] = digits[0];
	}
	/*Placing the value of buffer in the rest of the cells*/
	for (j = 0; j < strlen(bufferTmp); j++) {
		buffer[i + j] = bufferTmp[j];
	}

	/*It is a string so we have to close it with Null char*/
	buffer[i + j] = '\0';
}

void intToBase8(unsigned int num, char *result) {
	char *converter = "!@#$%^&*";
	char d[6];
	int i = 0;
	int j;
	int x = 0;
	if (num == 0) {
		result[0] = '!';
		result[1] = '\0';
		return;
	}
	while (num) {
		d[i] = converter[num % 8];
		i++;
		num = num / 8;
	}
	for (x = 0, j = i - 1; j >= 0; x++, j--) {
		result[x] = d[j];

	}

	result[x] = '\0';
}

/*The function updates the integer binary value to negative number with 13 bits*/
void isNegative(unsigned int *num) {
	unsigned int temp;
	int i, j;
	int bits[13];
	double x;
	temp = *num;
	*num = 0;
	j = 1;
	for (i = 0; i < 13; i++) {
		bits[i] = (temp & j);
		temp >>= ONE_BIT;
	}

	for (x = 0, i = 0; i < 13; i++) {
		x += bits[i] * (pow(2, i));
	}

	*num = (unsigned int) x;
}
/*The function updates the integer binary value to negative number with 15 bits*/
void isNegativeOn15Bits(unsigned int *num) {
	unsigned int temp;
	int i, j;
	int bits[15];
	double x;
	temp = *num;
	*num = 0;
	j = 1;
	for (i = 0; i < 15; i++) {
		bits[i] = (temp & j);
		temp >>= 1;
	}

	for (x = 0, i = 0; i < 15; i++) {
		x += bits[i] * (pow(2, i));
	}

	*num = (unsigned int) x;
}

/*The function moves all operand1 to opernd2 in case of group 2 (only one operand exists) */
void moveOper1to2() {
	extern int line;
	int i;
	char temp1[MAX_INPUT];
	int len;
	for (i = 1; i <= line; i++) {

		strcpy(temp1, getData(i, OPERAND2));
		if (strcmp(getData(i, COMMAND), DATALABEL)) {
			(len = strlen(temp1));
			if (!len) {
				setData(i, OPERAND2, getData(i, OPERAND1));
				setData(i, OPERAND1, temp1);
			} else {
				if (!strlen(temp1)) {
					setData(i, OPERAND2, getData(i, OPERAND1));
					setData(i, OPERAND1, temp1);
				}
			}
		}
	}
}

int extract(int value, int start, int end) {
	int mask = ~(~0 << (end - start + 1));

	return ((value >> start) & mask);
}

int getBitRange(int value, int start, int end) {
	unsigned int final_num;
	int temp, MSB;
	int i;
	final_num = extract(value, start, end);
	temp = final_num;
	for (i = 0; i < (start - end); i++)
		temp >>= 1;

	MSB = temp & 1;
	if (MSB) {
		temp = 0b01111111111;
		for (i = 0; i < ((end - start) + 1); i++)
			temp <<= 1;
		final_num = temp + final_num;
	}
	return final_num;

}

/*Free the memory tables*/
void freeTables() {
	free(symTable);
	free(outTable);
}

