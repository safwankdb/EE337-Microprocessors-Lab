ORG 00H
LJMP MAIN


ORG 100H
	bin2ascii:
	USING 0
		PUSH PSW
		PUSH AR0
		PUSH AR1
		PUSH AR2
		PUSH ACC
		
		MOV 50H, #1
		MOV 51H, #60H
		MOV 52H, #70H

		MOV R2, 50H
		MOV R1, 51H
		MOV R0, 52H


        MOV A, #0
        LOOP:
            ADD A, @R1
            INC R1
            DJNZ R2, LOOP
        CPL A
        ADD A, #1
        MOV R2, A
        
        ; Lower Nibble
        INC R0
        CLR C
		ANL A, #00FH
        SUBB A, #0AH
        JC NUM1
        ADD A, #41H
        SJMP STORE1
        NUM1:
            ADD A, #30H
			ADD A, #0AH
        STORE1:
            MOV @R0, A

        ;Upper Nibble
        MOV A, R2
        ANL A, #0F0H
        RR A
        RR A
        RR A
        RR A
        DEC R0
        CLR C
        SUBB A, #0AH
        JC NUM2
        ADD A, #41H
        SJMP STORE2
        NUM2:
            ADD A, #30H
			ADD A, #0AH
        STORE2:
            MOV @R0, A

		POP ACC
		POP AR2
		POP AR1
		POP AR0
		POP PSW
		RET
	
	
MAIN:
	LCALL bin2ascii
	END
