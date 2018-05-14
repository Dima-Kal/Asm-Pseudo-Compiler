
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "assembler.h"

extern int errCnt;
extern int sucCnt;
extern int iLine;
extern int yLine;	
extern char* fileName;
extern int jLine;

void error (errorType error)
{

	FILE *f = stderr;  
	errCnt++;
	
	switch (error)
	{

    case ERR_MEMORY_ALLOCATION:
	  fprintf(f, "[ERR:(%d)] : Memory allocation failed\n", errCnt);
  	  break; 

    case ERR_WRONG_INPUT:
	  fprintf(f, "[ERR:(%d)] : Wrong input in line %i\n", errCnt, iLine + yLine - 1);
  	  break;	 

    case ERR_MEMORY_MAX_LABEL:
	  fprintf(f, "[ERR:(%d)] : Memory crashed - the label on line %i is more then 30 characters\n",errCnt, iLine + yLine);
  	  break;

    case ERR_LABEL_CHARACTER:
	  fprintf(f, "[ERR:(%d)] : The first character on the label in line %i is wrong\n",errCnt, iLine + yLine);
  	  break; 

    case ERR_WRONG_LABEL:
      fprintf(f, "[ERR:(%d)] : Wrong character found on label in line %i\n",errCnt, iLine + yLine);
  	  break; 
	  
    case ERR_MEMORY_COMMAND:
      fprintf(f,"[ERR:(%d)] : Memory crashed - the Command on line %i is too long\n",errCnt, iLine + yLine);
  	  break; 

	case ERR_MEMORY_OPERAND1:
	  fprintf(f, "[ERR:(%d)] : Memory crashed - the operand1 on line %i is too long\n",errCnt, iLine + yLine);
  	  break;
	  
    case ERR_MEMORY_OPERAND2:
	  fprintf(f, "[ERR:(%d)] : Memory crashed - the operand2 on line %i is too long\n",errCnt, iLine + yLine);
  	  break; 

    case ERR_WRONG_OPERAND_CHARACTER:
	  fprintf(f, "[ERR:(%d)] : Wrong operand in line %i\n", errCnt, getInputLine(iLine));
  	  break; 
  
    case ERR_INDEX_OUT_OF_BOUND:
	  fprintf(f, "[ERR:(%d)] : Parameter or Index out of bound\n", errCnt);
  	  break; 

    case ERR_WRONG_PARAMETER:
	  fprintf(f, "[ERR:(%d)] : Parameter was not found\n", errCnt);
  	  break; 
	  
    case ERR_FILE_NOT_EXIST:
	  fprintf(f,"[ERR:(%d)] : The file %s doesn't exist\n", errCnt,fileName);
	  break;
	  

    case ERR_COMPILATION_FAILED: 
      fprintf(f, "[ERR:(%d)] : The file %s cannot compile.\n", errCnt,fileName);
      break;

    case ERR_WRONG_OPERAND: 
      fprintf(f, "[ERR:(%d)] : Wrong operand in line %i\n", errCnt, iLine);
      break;

    case ERR_WRONG_OPERAND_OR_COMMAND: 
      fprintf(f, "[ERR:(%d)] : Found wrong command or illegal operands in line %i\n",errCnt, getInputLine(iLine));
      break;

    case ERR_DUPLICATE_LABEL:
      fprintf(f,"[ERR:(%d)] : Found duplicate label in line %i and line %iLine\n",errCnt, getInputLine(iLine), getInputLine(jLine));
      break;

    case ERR_WRONG_VALUE:
	  fprintf(f, "[ERR:(%d)] : Wrong value in line %i \n",errCnt, getInputLine(iLine));
	  break;

    case ERR_WRONG_OPERAND_SYMBOL:
	  fprintf(f, "[ERR:(%d)] : Wrong operand in line %i, # must be followed by a number \n",errCnt, getInputLine(iLine));
      break;

    case ERR_NUMBER_OUT_OF_BOUND:
	  fprintf(f, "[ERR:(%d)] : Number out of bound in line %i\n",errCnt, getInputLine(iLine));
      break;

	case ERR_ILLEGAL_VALUE_TABLE:
	  fprintf(f, "[ERR:(%d)] : Illegal value in line %i col %i\n",errCnt, getInputLine(iLine), jLine);
      break;

    case ERR_ILLEGAL_VALUE_LINE:
	  fprintf(f, "[ERR:(%d)] : Illegal value in line %i\n", errCnt, getInputLine(iLine));
      break;
	
	case ERR_ILLEGAL_CHARACTER:
	  fprintf(f, "[ERR:(%d)] : Illegal character in line %i\n",errCnt, getInputLine(iLine));
      break;
      
	case ERR_NO_FILE:
	  fprintf(f, "[ERR:(%d)] : No files to compile\n",errCnt);
      break;     
      
	case ERR_NUMERIC_VALUE:
	  fprintf(f, "[ERR:(%d)] : Numeric value of the operand in line %i is out of bounds \n",errCnt, getInputLine(iLine));
      break;            
	}
}

void success (successType success)
{

	FILE *f = stderr;  
	sucCnt++;
	
	switch (success)
	{

    case SUCC_COMPILATION:
	  fprintf(f, "[SUCCESS:(%d)] : The file %s compiled.\n", sucCnt,fileName);
  	  break; 

    case SUCC_CREATE_OB_FILE:
	  fprintf(f, "[SUCCESS:(%d)] : The file %s.ob created.\n",sucCnt, fileName);
  	  break;	 

    case SUCC_CREATE_EX_FILE:
	  fprintf(f, "[SUCCESS:(%d)] : The file %s.ex created.\n",sucCnt, fileName);
  	  break;

    case SUCC_CREATE_EN_FILE:
	  fprintf(f, "[SUCCESS:(%d)] : The file %s.en created.\n",sucCnt, fileName);
  	  break; 

	}
}



