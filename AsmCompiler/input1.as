.entry LOOP
.entry LENGTH
.extern L3
.extern W
MAIN: mov K, W
	 add r2, STR
	mov #4, r4
LOOP: jmp L3
	prn #-5
	sub r2, r4
	inc r0
	mov r2, r3
	bne L3
END: stop
STR: .string "abcdef"
LENGTH: .data 6, -9 , 15
K: .data 2

