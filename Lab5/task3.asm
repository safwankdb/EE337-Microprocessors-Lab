; This subroutine writes characters on the LCD
LCD_data equ P2    ;LCD Data port
LCD_rs   equ P0.0  ;LCD Register Select
LCD_rw   equ P0.1  ;LCD Read/Write
LCD_en   equ P0.2  ;LCD Enable

ORG 0000H
ljmp start

org 100h
DELAY_5:
		MOV R4, #5
BACK3: LCALL DELAY_1
		DJNZ R4, BACK3		
		RET
		
		DELAY_1:
	   MOV R3, #20
	   BACK2: MOV R2, #200
	   BACK1: MOV R1, #0FFH
	   BACK: DJNZ R1, BACK
	   DJNZ R2, BACK1
	   DJNZ R3, BACK2
       RET
	   disp_a:
	   mov r0, a
	  swap a
	  anl a, #0fh
	  mov dptr, #bin2ascii
	  movc a, @a + dptr
	  acall lcd_senddata
	  mov a, r0
	  anl a, #0fh
	  mov dptr, #bin2ascii
	  movc a, @a + dptr
	  acall lcd_senddata   
	  ret
start:
      mov r7, #43h
	  mov b, #0e3h
	  mov r2, #0aah
	  mov r5, #67h
      mov P2,#00h
      mov P1,#00h
	  ;initial delay for lcd power up

	;here1:setb p1.0
      	  acall delay
	;clr p1.0
	  acall delay
	;sjmp here1


	  acall lcd_init      ;initialise LCD
	
	  acall delay
	  acall delay
	  acall delay
	  mov a,#80h		 ;Put cursor on first row,5 column
	  acall lcd_command	 ;send command to LCD
	  acall delay
	  mov   dptr,#my_string1   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  acall delay
	  lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine;mov   a, #''  ;Load DPTR with sring1 Addr
	  ;acall lcd_senddata	   ;call text strings sending routine
	  mov a,b
	  lcall disp_a
	 ; acall lcd_senddata
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,psw
	  lcall disp_a
	  acall delay
	  mov a,#0C0h		  ;Put cursor on second row,3 column
	  acall lcd_command
	  acall delay
	  mov   dptr,#my_string2   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  acall delay
	  mov a,r0
	  lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,r1
	  lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,r2
	  lcall disp_a
      acall DELAY_5
	  mov a,#80h		 ;Put cursor on first row,5 column
	 acall lcd_command	 ;send command to LCD
	  acall delay
	  mov   dptr,#my_string3   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  acall delay
	  mov a, r3
	   lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,r4
	   lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,r5
	   lcall disp_a
	  acall delay
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring
	  mov a,#0C0h		  ;Put cursor on second row,3 column
	  acall lcd_command
	  acall delay
	  mov   dptr,#my_string4   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  acall delay
	  mov a,r6
	   lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,r7
	  lcall disp_a
	  mov   dptr,#space   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  mov a,sp
	  lcall disp_a	  
here: sjmp here				//stay here 

;------------------------LCD Initialisation routine----------------------------------------------------
lcd_init:
         mov   LCD_data,#38H  ;Function set: 2 Line, 8-bit, 5x7 dots
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
	     acall delay

         mov   LCD_data,#0CH  ;Display on, Cursor off
         clr   LCD_rs         ;Selected instruction register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         
		 acall delay
         mov   LCD_data,#01H  ;Clear LCD
         clr   LCD_rs         ;Selected instruction register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         
		 acall delay

         mov   LCD_data,#06H  ;Entry mode, auto increment with no shift
         clr   LCD_rs         ;Selected instruction register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en

		 acall delay
         
         ret                  ;Return from routine

;-----------------------command sending routine-------------------------------------
 lcd_command:
         mov   LCD_data,A     ;Move the command to LCD port
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
		 acall delay
    
         ret  
;-----------------------data sending routine-------------------------------------		     
 lcd_senddata:
         mov   LCD_data,A     ;Move the command to LCD port
         setb  LCD_rs         ;Selected data register
         clr   LCD_rw         ;We are writing
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         acall delay
		 acall delay
         ret                  ;Return from busy routine

;-----------------------text strings sending routine-------------------------------------
lcd_sendstring:
	push 0e0h
	lcd_sendstring_loop:
	 	 clr   a                 ;clear Accumulator for any previous data
	         movc  a,@a+dptr         ;load the first character in accumulator
	         jz    exit              ;go to exit if zero
	         acall lcd_senddata      ;send first char
	         inc   dptr              ;increment data pointer
	         sjmp  LCD_sendstring_loop    ;jump back to send the next character
exit:    pop 0e0h
         ret                     ;End of routine

;----------------------delay routine-----------------------------------------------------
delay:	 push 0
	 push 1
         mov r0,#1
loop2:	 mov r1,#255
	 loop1:	 djnz r1, loop1
	 djnz r0, loop2
	 pop 1
	 pop 0 
	 ret

;------------- ROM text strings---------------------------------------------------------------
org 300h
my_string1:
         DB   "ABPSW = ", 00H
space:
         DB    " ", 00H			 
my_string2:
		 DB   "R012 = ", 00H
my_string3:
		 DB   "R345 = ", 00H
my_string4:
		 DB   "R67SP = ", 00H
bin2ascii:
         DB 30H, 31H, 32H, 33H, 34H, 35H, 36H, 37H, 38H, 39H, 41H, 42H, 43H, 44H, 45H, 46H
end

