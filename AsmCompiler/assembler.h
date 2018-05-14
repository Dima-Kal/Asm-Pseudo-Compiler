/*
Header file = assembler.h

 |=======================================================|
  |						 	|
  |	   By Dima Kailinichenko, (Id) 319606166	|
  |	     & Elad Ben-David, (Id) 300235066	 	|
  |							|
 |=======================================================|

 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*should present status*/
#define LABEL 1
#define COMMAND 2
#define OPERAND1 3
#define OPERAND2 4

#define MAX_INPUT 31
#define FIRST_ADDRESS 100
#define MAX_SIZE_BASE8_NUM 6 
#define	NUM_BITS_BASE8 3
#define	BASE_NUM 8
#define	MEM_WORD_SIZE 15

/*For every code line have between 1-3 different code lines */
#define FIRST_LINE 0
#define SECOND_LINE 1
#define THIRD_LINE 2

/*Operands binary values*/
#define EMPTY 0
#define NUMERIC 0
#define LABEL 1
#define DYNAMIC 2
#define REGISTER 3

/*Command groups*/
#define GROUP1 2
#define GROUP2 1
#define GROUP3 0

/*Bits*/
#define ONE_BIT 1
#define TWO_BITS 2
#define FOUR_BITS 4
#define SIX_BITS 6
#define UNUSED_BITS 5

/*Constants for Boolean*/
#define TRUE 1
#define FALSE 0

/*Direct Addressing Min and Max values*/
#define DIRECTMIN -4096 
#define DIRECTMAX 4095 

/*.Data lines min and max values*/
#define MINDATANUM -32768 
#define MAXDATANUM 65535 

#define COMMA 0
#define NUMBER 1

#define DATALABEL ".data"
#define STRINGLABEL ".string"

/*Sructures*/
/*This is the structure for a line of command*/

typedef struct {
	char label[MAX_INPUT];
	char command[MAX_INPUT];
	char operand1[MAX_INPUT];
	char operand2[MAX_INPUT];
	int inputLine;
	int dynamicRange;
	int dynStart;

} parsedLine;

/*Symbol table struct presents a line*/
typedef struct {
	char symbol[MAX_INPUT];
	int address;
	int dataAdress;
	char* label;
} symbolLine;

/*Output table struct presents a line*/
typedef struct {
	char symbol[MAX_INPUT];
	char dynOP[MAX_INPUT];
	int dynRange;
	int dynStart;
	int address;
	unsigned int code;
} outputLine;


typedef enum 
{
  ERR_MEMORY_ALLOCATION,
  ERR_WRONG_INPUT,
  ERR_MEMORY_MAX_LABEL,
  ERR_LABEL_CHARACTER,
  ERR_WRONG_LABEL,
  ERR_MEMORY_COMMAND,
  ERR_MEMORY_OPERAND1,
  ERR_MEMORY_OPERAND2, 
  ERR_WRONG_OPERAND_CHARACTER,
  ERR_INDEX_OUT_OF_BOUND,
  ERR_WRONG_PARAMETER,
  ERR_FILE_NOT_EXIST,
  ERR_COMPILATION_FAILED,
  ERR_WRONG_OPERAND,
  ERR_WRONG_OPERAND_OR_COMMAND,
  ERR_DUPLICATE_LABEL,
  ERR_WRONG_VALUE,
  ERR_WRONG_OPERAND_SYMBOL,
  ERR_NUMBER_OUT_OF_BOUND,
  ERR_ILLEGAL_VALUE_TABLE,
  ERR_ILLEGAL_VALUE_LINE,
  ERR_ILLEGAL_CHARACTER,
  ERR_NO_FILE,
  ERR_NUMERIC_VALUE
} errorType;

typedef enum 
{
  SUCC_COMPILATION,
  SUCC_CREATE_OB_FILE,
  SUCC_CREATE_EX_FILE,
  SUCC_CREATE_EN_FILE  
} successType;

/*Function prototype*/
/*input collection function*/
/*The function gets input from file, allocates memory and sorts them in a structure.*/
int buildParsedLine(FILE *file);
/*Gets data for a single parameter and index line*/
char* getData(int index, int param);
/*Gets the line number in the input file*/
int getInputLine(int i);
/*Sets data parameter on our parsed line */
void setData(int index, int param, char data[]);
/*Count lines of data for the memory allocation*/
int lineCounter(FILE *file);
/*Free memory of the global element*/
void freeParsedMemory();
/*Checking if in line <line> there is an element of type elementType*/
int isElementExist(int line, int elementType);
/*Get range of binary digits for dynamic addressing*/
int getDynamicAddressing(char operand[]);
/*Get Dynamic index*/
int getDynamic(int index);
/*Get Dynamic start range*/
int getDynamicStart(int index);
/*Get Dynamic Bits range*/
int getBitRange(int value, int start, int end);
/*Validation function*/
/*Check the command and operands in line i*/
int validateCommand(char com[], int i);
/*Check if all commands are valid */
int validateAllCommands();
/*Run all over the input and check each iteration for one label*/
int validateLabels();
/*Test for existing and legal label*/
int isLabel(char operand[]);
/*Checking if the data in the extern file */
int isExtern(char param[]);
/*Checking if the data in the entry file*/
int isEntry(char param[]);
/*check if is a register*/
int isRegister(char oper[]);
/*check if operand is a numeric value*/
int isNumeric(char operand[], int i);
/*check if operand is dynamic addressing*/
int isDynamic(char operand[], int i);
/*Check if .Data has legal values*/
int isData(int i);
/*Check if .String has legal values*/
int isString(int i);
/*Returns true if line i has a data or string label*/
int isDataLabel(int i);
/*Parser function*/
/*Function that creates the symbol table with the correct address*/
void createSymbolTable();
/*Creating the table with the output data*/
void createOutputTable();
/*The function returns the symbol address*/
int getSymbolAddress(char symbol[]);
/*Calculate how many code line the input line needed*/
int countLines(int i);
/*The function changes the value of integer to base 8*/
void intToBase8(unsigned int num, char *result);
/*The function changes the value of integer to base 8 for code*/
void intToBase8Code(int num, char *buffer);
/*The function updates the integer binary value to negative number with 13 bits*/
void isNegative(unsigned int *num);
/*The function updates the integer binary value to negative number with 15 bits*/
void isNegativeOn15Bits(unsigned int *num);
/*The function moves all operand1 to opernd2 in case of group 2 (only one operand exists) */
void moveOper1to2();
/*Free table memory*/
void freeTables();
/*helper functions*/
/*checks if <o> is a digit*/
int isDigit(char o);
/*checks if <x> is a space*/
int isSpace(char x);
/*checks if <x> is a letter*/
int isLetter(char x);
/*checks if the line with index <i> is empty*/
int isEmptyLine(int i);

/* print error */
void error (errorType error);
/* print success */
void success (successType success);

#endif

