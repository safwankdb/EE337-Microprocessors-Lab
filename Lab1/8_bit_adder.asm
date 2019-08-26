ORG 0000H
LJMP MAIN
ORG 50H
ADDER_8BIT:
	MOV A, R0
	ADD A, R1
	MOV 70H, A
	MOV A, #0
	MOV ACC.0, C
	MOV 71H, A
	RET
INIT:
	MOV 50H, #0xFF
	MOV 60H, #0xFF
	MOV R0, 50H
	MOV R1, 60H
	MOV A, #0
	RET
ORG 0100H
MAIN:
	ACALL INIT 		 ;Loading the values in memory
	ACALL ADDER_8BIT ;Adding the two values
LOOP:  
	SJMP LOOP
END
