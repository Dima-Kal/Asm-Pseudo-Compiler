/*
Validation file - validation.c

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
#include "assembler.h"

extern int errCnt;
extern int sucCnt;

int isDigit(char o) {
	return o >= '0' && o <= '9';
}

int isSpace(char x) {
	return x == ' ' || x == '\t';
}

int isLetter(char x) {
	return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z');
}

int isEmptyLine(int i) {
	return !strlen(getData(i, LABEL)) && !strlen(getData(i, OPERAND1)) && !strlen(getData(i, OPERAND2)) && !strlen(getData(i, COMMAND));
}
/*Check the command and operands in line i*/
int validateCommand(char com[], int i) {
	int flag = FALSE;

	/*Commands mov, add and sub
	 Must have two operands
	 Operand1 must be either Label, Register or legal numeric value
	 */

	if (isEmptyLine(i))
		return TRUE;

	if (!strcmp("mov", com) || !strcmp("add", com) || !strcmp("sub", com)) {
		/*Do we have two operands?*/
		if (isElementExist(i, OPERAND1) && isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isRegister(getData(i, OPERAND1)) || isLabel(getData(i, OPERAND1)) || isNumeric(getData(i, OPERAND1), i) || isDynamic(getData(i, OPERAND1), i)) {
				/*Is Operand2 legal?*/
				if (isRegister(getData(i, OPERAND2)) || isLabel(getData(i, OPERAND2))) {
					flag = TRUE;
				} else {
					error(ERR_WRONG_OPERAND);
					flag = FALSE;
				}

			} else {
				error(ERR_WRONG_OPERAND);
				flag = FALSE;
			}
		}

		else {
			flag = FALSE;
		}
	}

	/*Command cmp
	 Must have two operands
	 Operands 1 and 2 must be either Label, Register or legal numeric value
	 */
	if (!strcmp("cmp", com)) {
		/*Do we have two operands?*/
		if (isElementExist(i, OPERAND1) && isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isRegister(getData(i, OPERAND1)) || isLabel(getData(i, OPERAND1)) || isNumeric(getData(i, OPERAND1), i)) {
				/*Is Operand2 legal?*/
				if (isRegister(getData(i, OPERAND1)) || isLabel(getData(i, OPERAND1)) || isNumeric(getData(i, OPERAND1), i))
					flag = TRUE;
				else
					flag = FALSE;

			} else
				flag = FALSE;
		}

		else
			flag = FALSE;
	}

	/*Command lea
	 Must have two operands
	 Operand 1 must be a label
	 Operand 2 must be either register or label
	 */
	if (!strcmp("lea", com)) {
		/*Do we have two operands?*/
		if (isElementExist(i, OPERAND1) && isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isLabel(getData(i, OPERAND1))) {
				/*Is Operand2 legal?*/
				if (isRegister(getData(i, OPERAND1)) || isLabel(getData(i, OPERAND1))) {
					flag = TRUE;
				} else {
					flag = FALSE;
				}
			} else {
				flag = FALSE;
			}
		}

		else {
			flag = FALSE;
		}
	}
	/*Command not
	 Must have one operand
	 Operands 1 must be  register
	 */
	if (!strcmp("not", com)) {
		/*Do we have one operands?*/
		if (isElementExist(i, OPERAND1) && !isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isRegister(getData(i, OPERAND1))) {
				flag = TRUE;
			} else {
				flag = FALSE;
			}
		} else {
			flag = FALSE;
		}
	}
	/*Commands clr, inc. dec, jmp, bne, red
	 Must have one operands
	 Operands 1 must be either Register or Label
	 */
	if (!strcmp("clr", com) || !strcmp("inc", com) || !strcmp("dec", com)
			|| !strcmp("jmp", com) || !strcmp("bne", com)
			|| !strcmp("red", com)) {
		/*Do we have one operands?*/
		if (isElementExist(i, OPERAND1) && !isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isRegister(getData(i, OPERAND1))
					|| isLabel(getData(i, OPERAND1))) {
				flag = TRUE;
			} else {
				flag = FALSE;
			}
		} else {
			flag = FALSE;
		}
	}
	/*Command prn
	 Must have one operand
	 Operands 1 must be either register, label or numeric
	 */
	if (!strcmp("prn", com)) {
		/*Do we have one operands?*/
		if (isElementExist(i, OPERAND1) && !isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isRegister(getData(i, OPERAND1))
					|| isLabel(getData(i, OPERAND1))
					|| isNumeric(getData(i, OPERAND1), i)) {
				flag = TRUE;
			} else {
				flag = FALSE;
			}
		} else {
			flag = FALSE;
		}
	}
	/*Command jsr
	 Must have one operand
	 Operands 1 must be label
	 */
	if (!strcmp("jsr", com)) {
		/*Do we have one operands?*/
		if (isElementExist(i, OPERAND1) && !isElementExist(i, OPERAND2)) {
			/*Is Operand1 legal?*/
			if (isLabel(getData(i, OPERAND1))) {
				flag = TRUE;
			} else {
				flag = FALSE;
			}
		} else {
			flag = FALSE;
		}
	}
	/*
	 Commands rts and stop
	 Must have no operands
	 */
	if (!strcmp("rts", com) || !strcmp("stop", com)) {
		if (!isElementExist(i, OPERAND1) && !isElementExist(i, OPERAND2))
			flag = TRUE;
		else {
			flag = FALSE;
		}
	}
	/*.data tests*/
	if (!strcmp(DATALABEL, com)) {
		if (isData(i))
			flag = TRUE;
		else {
			flag = FALSE;
		}
	}
	/*.string tests*/
	if (!strcmp(STRINGLABEL, com)) {
		if (isString(i))
			flag = TRUE;
		else {
			flag = FALSE;
		}
	}
	if (flag == FALSE) {
		error(ERR_WRONG_OPERAND_OR_COMMAND);
	}
	return flag;
}

/*Check if all commands are valid */
int validateAllCommands() {
	extern int line;
	int i = 1;
	int flag = TRUE;
	while (i <= line) {
		if (!validateCommand(getData(i, COMMAND), i))
			flag = FALSE;
		i++;
	}

	return flag;
}
/*Run all over the input and check each iteration for one label*/
int validateLabels() {
	char temp1[31];
	char temp2[31];
	extern int line;
	int i = 1;
	int j = 1;
	int flag = TRUE;
	while (i <= line) {
		strcpy(temp1, getData(i, LABEL));
		while (j <= line) {
			strcpy(temp2, getData(j, LABEL));
			if (j != i && strlen(temp1) != 0 && strlen(temp2) != 0) {
				if (strcmp(temp1, temp2) == 0) {
					flag = FALSE;
					error(ERR_DUPLICATE_LABEL);
				}
			}
			j++;
		}
		i++;
	}
	return flag;
}

/*Test for existing and legal label*/
int isLabel(char operand[]) {
	extern int line;
	int i = 1;
	while (i <= line) {
		/*Check in the label column*/
		if (strcmp(getData(i, LABEL), operand) == 0) {

			if (strcmp(getData(i, COMMAND), STRINGLABEL) == 0)
				return TRUE;
			else if (strcmp(getData(i, COMMAND), DATALABEL) == 0)
				return TRUE;

		}
		i++;
	}
	return isExtern(operand);
}
/*Checking if the data in the extern file */
int isExtern(char param[]) {
	FILE *fp;
	char c[MAX_INPUT];
	char fileExt[FILENAME_MAX];
	extern char* fileName;
	strcpy(fileExt, fileName);
	strcat(fileExt, ".ex");
	fp = fopen(fileExt, "r");
	if (!fp)
		return FALSE;
	fseek(fp, SEEK_SET, 0);
	while ((fscanf(fp, "%s", c)) == 1) {
		if (!strcmp(param, c)) {
			fclose(fp);
			return TRUE;
		}
	}
	fclose(fp);
	return FALSE;
}
/*Checking if the data in the entry file*/
int isEntry(char param[]) {
	FILE *fp;
	char c[MAX_INPUT];
	char fileEnt[FILENAME_MAX];
	extern char* fileName;
	strcpy(fileEnt, fileName);
	strcat(fileEnt, ".en");
	fp = fopen(fileEnt, "r");
	if (!fp)
		return FALSE;
	fseek(fp, SEEK_SET, 0);
	while ((fscanf(fp, "%s", c)) == 1) {
		if (!strcmp(param, c)) {
			fclose(fp);
			return TRUE;
		}

	}
	fclose(fp);
	return FALSE;
}
/*check if is a register*/
int isRegister(char oper[]) {
	if (!strcmp(oper, "r0") || !strcmp(oper, "r1") || !strcmp(oper, "r2") || !strcmp(oper, "r3") || !strcmp(oper, "r4") || !strcmp(oper, "r5") || !strcmp(oper, "r6") || !strcmp(oper, "r7"))
		return TRUE;
	return FALSE;
}
/*check if operand is a numeric value*/
int isNumeric(char oper[], int i) {
	int flag = FALSE;
	int length = strlen(oper);
	int value;
	int j;

	if (oper[0] == '#') {
		if (length < 2) {
			error(ERR_WRONG_OPERAND_CHARACTER);
		} else if (length == 2) {
			if (isDigit(oper[1]))
				flag = TRUE;
			else {
				error(ERR_WRONG_OPERAND_SYMBOL);
			}
		} else if (length > 2) {
			if (oper[1] == '-' || oper[1] == '+' || isDigit(oper[1])) {
				flag = TRUE;
				for (j = 2; j < length; j++) {
					if (!(isDigit(oper[j]))) {
						flag = FALSE;
						error(ERR_WRONG_VALUE);
					}
				}
			} else {
				error(ERR_WRONG_OPERAND_SYMBOL);
			}
			/*Check that the numeric value is in range*/
			value = strtol(oper + 1, NULL, 10);

			if (value > DIRECTMAX || value < DIRECTMIN) {
				error(ERR_NUMERIC_VALUE);
				flag = FALSE;
			}
		}
	}/*end number*/
	return flag;
}

/*Check if .Data has legal values*/
int isData(int i) {
	char str[31];
	int state = COMMA;
	int len;
	int bigflag = TRUE;
	int flag;
	int j = 0;
	int num;
	strcpy(str, getData(i, OPERAND1));
	len = strlen(str);
	if (len > 0) {
		while (j < len) {
			if (isDigit(str[j]) || str[j] == '+' || str[j] == '-') {
				num = strtol(str + j, NULL, 10);
				if (num < MINDATANUM || num > MAXDATANUM) {
					error(ERR_NUMBER_OUT_OF_BOUND);
					bigflag = FALSE;
				}
			} else {
				bigflag = FALSE;
				error(ERR_ILLEGAL_VALUE_TABLE);
			}
			j++;
		}
	}
	j = 0;
	state = COMMA;
	strcpy(str, getData(i, OPERAND2));
	len = strlen(str);
	if (len > 0) {
		while (j < len) {
			flag = FALSE;
			if (state == COMMA && flag == FALSE) {
				if (isDigit(str[j]) || str[j] == '+' || str[j] == '-') {
					state = NUMBER;
					flag = TRUE;
					num = strtol(str + j, NULL, 10);
					if (num < MINDATANUM || num > MAXDATANUM) {
						error(ERR_NUMBER_OUT_OF_BOUND);
						bigflag = FALSE;
					}
				}
				if (str[j] != ' ' && state == COMMA && flag == FALSE) {
					error(ERR_ILLEGAL_VALUE_TABLE);
					bigflag = FALSE;
				}
			}
			if (state == NUMBER && flag == FALSE) {
				if (!isDigit(str[j])) {
					if (str[j] == ',') {
						state = COMMA;
						flag = TRUE;
					}
					if (str[j] != ' ' && state == NUMBER && flag == FALSE) {
						error(ERR_ILLEGAL_VALUE_TABLE);
						bigflag = FALSE;
					}
				}
			}
			j++;
		}
	}
	return bigflag;
}
/*Check if .String has legal values*/
int isString(int i) {
	char str[31];
	int flag = FALSE;
	int len;

	strcpy(str, getData(i, OPERAND1));
	len = strlen(str);

	if (str[0] == '"' && str[len - 1] == '"')
		flag = TRUE;

	return flag;
}

int isDataLabel(int i) {
	if (isElementExist(i, LABEL)) {
		if (strcmp(getData(i, COMMAND), STRINGLABEL) == 0)
			return TRUE;
		else if (strcmp(getData(i, COMMAND), DATALABEL) == 0)
			return TRUE;
	}
	return FALSE;

}

int isElementExist(int line, int elementType) {
	return strlen(getData(line, elementType));
}

