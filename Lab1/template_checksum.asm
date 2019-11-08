ORG 00H
;-- code memory location 00h
LJMP MAIN

ORG 50H
;-- code memory location 50h

INIT:
	;-- store the numbers to be added at appropriate locations
	MOV R0, #28H
	MOV R1, #40H
	MOV R2, #0FFH
	
	LOOP0:
		MOV A, R2
		MOV @R1, A
		INC R1
		;INC R2
		DJNZ R0, LOOP0
	RET
;-- end of subroutine INIT

ADD_40:
	;-- add the numbers stored from memory location 40h to 67h
	;-- by using subroutine written in homework
	MOV R0, #28H
	MOV R1, #40H
	MOV A, #0
	LOOP1:
		ADD A, @R1
		INC R1
		DJNZ R0, LOOP1
	RET
;-- end of subroutine ADD_40
	
TWOS_COMP:
	;-- perform 2's compliment of the resultant sum
	;-- store the checksum byte at memory location 68h
	CPL A
	ADD A, #1
	MOV 68H, A
	RET
;-- end of subroutine TWOS_COMP

ADD_41:
        ;-- add numbers from memory location 40h to 68h
	MOV R0, #29H
	MOV R1, #40H
	MOV A, #0
	LOOP2:
		ADD A, @R1
		INC R1
		DJNZ R0, LOOP2
	RET
;-- end of subroutine ADD_41

ORG 0200H
;-- code memory location 200h
MAIN:
	ACALL INIT
	ACALL ADD_40
	ACALL TWOS_COMP
	ACALL ADD_41	;verify the result
	HERE:SJMP HERE
END
