
;netID's; mkhan331, salkha21, jalenc3
;The program starts by going into the evaluate subroutine. The subroutine starts by
;taking in an input and echoing it to the screen. It then checks if an equal sign is entered. As long
;an equal sign is not entered, it checks if the input is a space and then a valid character (0-9 and
;all operations). It then pushes the number entered, to the stack (ASCII to DEC conversion) and takes in
;another input until an operator (+-/^*) is entered. If an operator is entered, the 2 top values of
;stack are popped, and then the desired operation is performed. If there are more inputs, the process
;repeats until an equal sign or invalid input is met. If there is one value in the stack after an
;= sign, the value in R5 (decimal) is converted to ASCII characters that are 
;printed in HEX format. If the input is invalid, "Invalid expression" is printed.

.ORIG x3000	         	
JSR EVALUATE				;The program goes straight into the evaluate subroutine

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0- value to print in hexadecimal

PRINT_HEX
; R1 >>> Bit Counter
; R2 >>> Digit Counter
; R3 >>> Digit
; R4 >>> Arithmetic
; R0 >>> Used to output

AND R2, R2, #0		  ; Initializes R2
ST R0, SAVE_R5		  ; This saves the output from R0 into an address so that it can be
					  ; Be put into R5 at the end of the program. 

EXCLUDE         AND R4, R4, #0        ;    Clear R4
                ADD R4, R2, #0        ;    Add digit counter into R4
                ADD R4, R4, #-4       ;    Subtracting 4 to check if < 4 digits
                BRnp LOOP_ONE         ;    If negative or positive, branch to LOOP_ONE
				BRnzp DONE_ONE		  ;    Branch to halt if done

LOOP_ONE        AND R3, R3, #0        ;    Clears Digit
                AND R1, R1, #0        ;    Clears Bit counter

COPY
                AND R4, R4, #0        ;    Clear R4
                ADD R4, R1, #0        ;    Add bit counter into R4
                ADD R4, R4, #-4       ;    Check if the bit counter is 0
                BRz LOOP_TWO          ;    Branch to LOOP_TWO
                ADD R5, R5, #0        ;    Checking for next bit from memory address
                BRn ADD_ONE           ;    Branch to ADD_ONE if negative
                ADD R3, R3, R3        ;    Shift left
                ADD R3, R3, #0        ;    Adding 0 to digit
                BRnzp SHIFT           ;    Branch to SHIFT


ADD_ONE         ADD R3, R3, R3        ;    Shift left digit
                ADD R3, R3, #1        ;    Adds 1 to digit    

SHIFT           ADD R5, R5, R5        ;    Left shift R5
                ADD R1, R1, #1        ;    Increment bit counter
                BRnzp COPY            ;    Branch to COPY        

LOOP_TWO        ADD R1, R1, #0        ;   Clear bit counter
				AND R4, R4, #0		  ;   Clear artihmetic
				ADD R4, R3, #0 		  ;   Add digit into arithmetic
                ADD R4, R4, #-9       ;   Check if digit <= 9 bits
                BRnz ADD_ZERO         ;   If zero or negative, branch to ADD_ZERO 
				AND R4, R4, #0		  ;   Clear arithmetic 
                LD R4, ASCIIA         ;   Load ASCII of 'A' into R4
                ADD R3, R3, #-10      ;   Subtracts 10    
                ADD R3, R4, R3        ;   Adds value of digit (A-10)
                BRzp FINISH           ;   Branch to FINISH

ADD_ZERO        ADD R4, R4, #0        ;   Clear R4
                AND R1, R1, #0        ;   Reset bit counter
                LD R4, ASCIIO         ;   Loads '0'
                ADD R3, R4, R3        ;   Adds '0' onto digit
FINISH        
                AND R0, R0, #0        ;   Clears R0
                ADD R0, R3, #0        ;   Adds R3 into R0    
                OUT                   ;   Output char    (digit)
                ADD R2, R2, #1        ;   Increments digit counter
                BRnzp EXCLUDE         ;   Branches to EXCLUDE


SAVE_R5     .BLKW #1 	;			  ; This holds the address that saves R5

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output


EVALUATE
;R0 is used as an output register and also checks for ASCII input char
;R1 is used to load negation of equal sign and checks for operators
;R2 is used to load negation of space
;R3 is used as an input for the subroutines
;R4 is used as an input for the subroutines
;R5 is used to check for underflow
NEXT
	AND R1, R1, #0   				; Initialize R1
	AND R2, R2, #0 					; Initialize R2
	LD R1, EQUAL_ASCII              ; Load equal sign
	LD R2, SPACE					; Load space
	NOT R1, R1   					; Negating equal sign
	ADD R1, R1, #1					; ^^^^^^
	NOT R2, R2						; Negating SPACE
	ADD R2, R2, #1					; ^^^^^^
	AND R0, R0, #0					;Initialize R0
	GETC							;Gets the input from the keyboard
	OUT								;Echos the input from the keybaord onto the screen							
	ST R0, SAVE_R0					;Stores the value of R0 for later
	LD R0, SAVE_R0					;Loads the value of R0 from address. 
	ADD R0, R0, R1                  ;Check for equal sign      				
	BRz CHECK_STACK                 ;Checks for equal sign
	LD R0, SAVE_R0					;Restores R0
	ADD R0, R0, R2					;Checking for space
	BRz NEXT						;Loops back to next character
	LD R0, SAVE_R0					;Reseting R0 to the original value
	LD R3, UNDER_0					;Sets R3 to the value of 0 in ASCII
	ADD R0, R0, R3					;Checks if ASCII value is less than 0
	BRnz CHECK_OP					;Loop to check operands
	LD R0, SAVE_R0                  ;Restores R0
	LD R3, ABOVE_9 					;Load negation of ASCII value of 9
	ADD R0, R0, R3 					;Check input char is above 9
	BRp CHECK_POWER					;If it is positive check for power
	BRnzp START_PUSH                ;Push values
CHECK_OP
	LD R0, SAVE_R0  				;RESET R0
	LD R3, P_SIGN					;Loads negation of positive sign ASCII
	ADD R0, R0, R3					;Checks for positive ASCII sign
	BRz START_POP					;Goes to pop if true
	
	LD R0, SAVE_R0  				;RESET R0			
	LD R3, N_SIGN					;Loads negation of negative sign ASCII
	ADD R0, R0, R3					;Checks for negative ASCII sign
	BRz START_POP					;Goes to pop if true

	LD R0, SAVE_R0  				;RESET R0			
	LD R3, M_SIGN					;Loads negation of multiply sign ASCII
	ADD R0, R0, R3					;Checks for multiply ASCII sign
	BRz START_POP					;Goes to pop if true
	
	LD R0, SAVE_R0  				;RESET R0			
	LD R3, D_SIGN					;Loads negation of divide sign ASCII
	ADD R0, R0, R3					;Checks for divide ASCII sign
	BRz START_POP					;Goes to pop if true
	
	BRnzp INVALID_EXP				;expression does not exist otherwise
	
CHECK_POWER
	LD R0, SAVE_R0  				;RESET R0			
	LD R3, CARROT					;Loads negation of carrot sign ASCII
	ADD R0, R0, R3					;Checks for carrot ASCII sign
	BRz START_POP					;Goes to pop if true
	
	BRnzp INVALID_EXP				;expression does not exist otherwize
	

START_PUSH
	LD R0, SAVE_R0					;Loads back value of R0
	ADD R0, R0, #-15				;Subtracts 48 for ASCII to decimal coversion
	ADD R0, R0, #-15				;^^^^^^^^
	ADD R0, R0, #-15				;^^^^^^^^
	ADD R0, R0, #-3					;^^^^^^^^
	ST R7, SAVE_R7					;Stores R7
	JSR PUSH						;Subroutines to push
	LD R7, SAVE_R7   				;Loads R7 back
	BRnzp NEXT						;Branches to next

START_POP	
	ST R7, SAVE_R7					;Stores R7
	JSR POP							;Subroutines to POP
	LD R7, SAVE_R7					;Loads back R7
	ADD R5, R5, #0					;Check for stack underflow
	BRp INVALID_EXP					;If it is go to invalid expression
	ADD R4, R0, #0					;Puts popped value into R4
	ST R7, SAVE_R7					;Stores R7
	JSR POP 						;Subroutines to POP again
	LD R7, SAVE_R7					;Loads back R7
	ADD R3, R0, #0					;Puts popped value into R3
	ADD R5, R5, #0					;Check for stack underflow
	BRp INVALID_EXP					;If it is go to invalid expression
	BRnzp APPLY						;Apply operand

DONE_ONE
	LD R5, SAVE_R5					;Loads output into R5
	BRnzp DONE						;Branches to DONE

APPLY
	LD R0, SAVE_R0  				;RESET R0 
	LD R1, P_SIGN					;Loads negation of positive sign ASCII
	ADD R0, R0, R1					;Checks for positive ASCII sign
	BRz START_ADD					;Goes to pop if true
	
	LD R0, SAVE_R0  				;RESET R0			
	LD R1, N_SIGN					;Loads negation of negative sign ASCII
	ADD R0, R0, R1					;Checks for negative ASCII sign
	BRz START_SUB					;Goes to pop if true

	LD R0, SAVE_R0  				;RESET R0			
	LD R1, M_SIGN					;Loads negation of multiply sign ASCII
	ADD R0, R0, R1					;Checks for multiply ASCII sign
	BRz START_MULT					;Goes to pop if true
	
	LD R0, SAVE_R0  				;RESET R0			
	LD R1, D_SIGN					;Loads negation of divide sign ASCII
	ADD R0, R0, R1					;Checks for divide ASCII sign
	BRz START_DIV					;Goes to pop if true

	LD R0, SAVE_R0  				;RESET R0			
	LD R1, CARROT					;Loads negation of divide sign ASCII
	ADD R0, R0, R1					;Checks for divide ASCII sign
	BRz START_EXP					;Goes to pop if true


START_ADD
	ST R7, SAVE_R7					;Stores R7
	JSR PLUS						;Subroutines to PLUS
	LD R7, SAVE_R7					;Loads R7 back
	ST R0, RESULT					;Stores R0 to RESULT
	BRnzp START_RESULT				;Branches to RESULT


START_SUB
	ST R7, SAVE_R7					;Stores R7
	JSR MIN							;Subroutines subtract subroutine
	LD R7, SAVE_R7					;Loadds back R7
	BRnzp START_RESULT				;Branches to start result



START_MULT
	ST R7, SAVE_R7					;Stores R7
	JSR MUL							;Subroutines to multiply subroutine
	LD R7, SAVE_R7					;Loads back R7
	BRnzp START_RESULT				;Branches to start result



START_DIV
	ST R7, SAVE_R7					;Stores R7
	JSR DIV  						;Subroutines to division subroutine
	LD R7, SAVE_R7					;Loads back R7
	BRnzp START_RESULT				;Branches to start result




START_EXP
	ST R7, SAVE_R7					;Stores R7 
	JSR EXP 						;branches to exponent subroutine
	LD R7, SAVE_R7					;Loads back R7
	BRnzp START_RESULT				;Branches to start result

START_RESULT						
	ST R0, RESULT					;Stores R0 to result address address for later
	LD R0, RESULT					;Loads back R0 from result address
	ST R7, SAVE_R7					;Stores R7
	JSR PUSH						;Branches into push subroutine 
	LD R7, SAVE_R7   				;Loads back R7
	BRnzp NEXT						;Branches back to read next character 

CHECK_STACK							
	ST R7, SAVE_R7					;Stores R7
	JSR POP							;POPs the value
	LD R7, SAVE_R7					;Loads back R7
	ST R7, SAVE_R7					;Stores R7
	JSR POP							;POPs the value
	LD R7, SAVE_R7					;Loads back R7
	ADD R5, R5, #0					;Check for underflow of that value by checking R5
	BRnz INVALID_EXP				;If it is not invalid expression
	ADD R5, R0, #0					;Check for underflow of that value by checking R5
	BRnzp PRINT_HEX					;Branches to print the result.		

INVALID_EXP	
	LEA R0, INVALID			;Loads string of invalid expression into R0
	PUTS					;Outputs string to the screen
	BRnzp DONE				;Branches to DONE for halting					
				
	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
AND R0, R0, #0 ; Clears R0
ADD R0, R3, R4 ; Adds value of R3 and R4

RET	           
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
AND R0, R0, #0  ; Clears R0
NOT R4, R4 		; Negates value in R4
ADD R4, R4, #1  ; ^^^^^^^^^
ADD R0, R3, R4  ; Subtract R3-R4     
						
RET             
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
AND R0, R0, #0 ;Clears R0

LOOP 
	 ADD R0, R0, R3 	;Adds value of R3 into R0. 
	 ADD R4, R4, #-1 	;Does avobe step (R4 amount of times)
	 BRnp LOOP			;Loops back to Adds R3 into R0 until R4 is zero
	 BRnzp STOP			;Loops back to return

STOP

RET
                              
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV

AND R5, R5, #0		; Clears R5
AND R1, R1, #0      ; Clears R1
AND R0, R0, #0 		; Clears R0
AND R2, R2, #0 		; Clears R2
ADD R2, R4, #0 		; Adds R4 into R2 to have a negated version of R4
NOT R2, R2      	; Negates R2
ADD R2, R2, #1      ; ^^^^^^^^^^^
ADD R5, R3, R2      ; Subtracts R3-R4
BRn SKIP            ; If it is negative branch to skip
	
SUB	ADD R3, R3, R2  ; Repeated subtraction
	ADD R3, R3, #0  ;
	BRn SKIP        ; If it is negative after the first subtraction branch to SKIP
	ADD R1, R1, #1  ; Increments R1 
	ADD R3, R3, #0  ; Checks if R3 is negative
	BRnz NEN        ; If it is negative or zero branch to NEN
	BRnzp SUB       ; If it is not branch back to repeates subtraction
NEN
		ADD R3, R3, #0  ; Checks if R3 is zero
		BRz SKIP        ; If value is zero branch to SKIP
		ADD R1, R1, #-1 ; Decrements R1
SKIP    ADD R0, R1, #0  ; Adds R1 to R0	

		RET	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0

EXP
AND R2, R2, #0            ; Clears R0
AND R0, R0, #0            ; Clears R0
ADD R2, R3, #0            ; Adds R3 into R2 for later
AND R1, R1, #0            ; Clears R1
ADD R4, R4, #-1           ; Starts input of R4 as (n-1)

SET
	  AND R1, R1, #0     ; Clears R1
	  ADD R1, R3, #0     ; Clears R3

LOOP2 ADD R0, R0, R3     ; Adds R3 into R0
	  ADD R1, R1, #-1    ; Decrements R1
	  BRnp LOOP2         ; Branches to LOOP2 if multiplication by addition is done.
	  ADD R4, R4, #-1    ; Decrements R4
	  BRp ANOTHER        ; If R4 is positive branch to ANOTHER
	  BRz STOP2          ; Else the subroutine returns.
 
ANOTHER
	  ADD R3, R0, #0     ; Adds R0 into R3
	  ADD R1, R2, #0     ; Adds R2 into R1
	  ADD R1, R1, #-1    ; Decrements R1
	  BRnzp LOOP2        ; Branches back to LOOP2 for more multiplication

STOP2                    
RET						 
;LINES 378 THROUGH 427 WERE NOT WRITTEN BY ME	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		    ;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		    ;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET

SPACE       .FILL x0020  ;
ASCIIO      .FILL x0030  ;
ASCIIA      .FILL x0041  ;
SAVE        .BLKW #1     ;
EQUAL_ASCII .FILL x003D  ;
UNDER_0     .FILL xFFD1  ;
ABOVE_9     .FILL xFFC7  ;
P_SIGN		.FILL xFFD5  ;
N_SIGN		.FILL xFFD3  ;
M_SIGN		.FILL xFFD6	 ;
D_SIGN		.FILL xFFD1  ;
CARROT      .FILL xFFA2  ;
INVALID     .STRINGZ " Invalid Expression" ; 

SAVE_R3		.BLKW #1	;
POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;
SAVE_R0     .BLKW #1	;
SAVE_R7     .BLKW #1    ;
RESULT      .BLKW #1	;

DONE  HALT              ; HALT


.END
