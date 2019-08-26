ORG 00H
	LJMP MAIN
DELAY:
	MOV R3, #5
	BACK0:
		MOV R2, #200
		BACK1:
			MOV R1, #0FFH
			BACK:
				DJNZ R1, BACK
			DJNZ R2, BACK1
		DJNZ R3, BACK0
	DJNZ R0, DELAY
	RET


ORG 100H

MAIN:
	ANL P1, #0F0H
	ORL P1, #1
	MOV A, P1
	ANL A, #00FH
	MOV R0, A
	LCALL DELAY
	CPL P1.5
	MOV R0, A
	LCALL DELAY
	CPL P1.5
	CPL P1.6
	MOV R0, A
	LCALL DELAY
	CPL P1.5
	MOV R0, A
	LCALL DELAY
	CPL P1.5
	CPL P1.6
	CPL P1.7
	LJMP MAIN
END