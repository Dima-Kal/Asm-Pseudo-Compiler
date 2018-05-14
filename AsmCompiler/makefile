
$(info )
$(info #######################################################)
$(info # 						      #)
$(info # General MakeFile for the assembler compiler program #)
$(info # 						      #)
$(info #                                                     #)
$(info #=====================================================#)
$(info #      By Dima Kailinichenko, (Id) 319606166 	      #)
$(info #	& Elad Ben-David, (Id) 300235066 	      #)
$(info #=====================================================#) 
$(info #					 	      #)
$(info #					 	      #)
$(info #######################################################)

EXEC_FILE = main
C_FILES = main.c input_reader.c parser.c validation.c error.c 
H_FILES = assembler.h

O_FILES = $(C_FILES:.c=.o)

all: $(EXEC_FILE)
$(EXEC_FILE): $(O_FILES) 
	gcc -Wall -ansi -pedantic $(O_FILES) -o $(EXEC_FILE)  -lm
%.o: %.c $(H_FILES)
	gcc -Wall -ansi -pedantic -c -o $@ $<
clean:
	rm -f *.o $(EXEC_FILE)
