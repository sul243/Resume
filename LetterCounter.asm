;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


    .ORIG    x3000        ; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

    LD R0,HIST_ADDR          ; point R0 to the start of the histogram
    
    ; fill the histogram with zeroes 
    AND R6,R6,#0        	; put a zero into R6
    LD R1,NUM_BINS        	; initialize loop count to 27
    ADD R2,R0,#0        	; copy start of histogram into R2

    ; loop to fill histogram starts here
HFLOOP  STR R6,R2,#0        ; write a zero into histogram
    	ADD R2,R2,#1        ; point to next histogram entry
    	ADD R1,R1,#-1       ; decrement loop count
    	BRp HFLOOP          ; continue until loop count reaches zero

    ; initialize R1, R3, R4, and R5 from memory
    LD R3,NEG_AT        	; set R3 to additive inverse of ASCII '@'
    LD R4,AT_MIN_Z        	; set R4 to difference between ASCII '@' and 'Z'
    LD R5,AT_MIN_BQ        	; set R5 to difference between ASCII '@' and '`'
    LD R1,STR_START        	; point R1 to start of string

    ; the counting loop starts here
COUNTLOOP
    LDR R2,R1,#0          ; read the next character from the string
    BRz PRINT_HIST        ; found the end of the string

    ADD R2,R2,R3          ; subtract '@' from the character
    BRp AT_LEAST_A        ; branch if > '@', i.e., >= 'A'
NON_ALPHA
    LDR R6,R0,#0        ; load the non-alpha count
    ADD R6,R6,#1        ; add one to it
    STR R6,R0,#0        ; store the new non-alpha count
    BRnzp GET_NEXT      ; branch to end of conditional structure
AT_LEAST_A
    ADD R6,R2,R4        ; compare with 'Z'
    BRp MORE_THAN_Z     ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA ADD R2,R2,R0        ; point to correct histogram entry
      LDR R6,R2,#0        ; load the count
      ADD R6,R6,#1        ; add one to it
      STR R6,R2,#0        ; store the new count
      BRnzp GET_NEXT      ; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
    ADD R2,R2,R5        ; subtract '`' - '@' from the character
    BRnz NON_ALPHA      ; if <= '`', i.e., < 'a', go increment non-alpha
    ADD R6,R2,R4        ; compare with 'z'
    BRnz ALPHA        	; if <= 'z', go increment alpha count
    BRnzp NON_ALPHA     ; otherwise, go increment non-alpha

GET_NEXT
    ADD R1,R1,#1        ; point to next character in string
    BRnzp COUNTLOOP        ; go to start of counting loop

; MY CONTRIBUTION TO THE CODE STARTS HERE

PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments

;Partners: mkhan331, salkha21, jalenc3
;Register Table
;R0 - Point R0 to the start of the histogram/Temp register
;R1 - Digit counter
;R2 - Digit
;R3 - Holds the additive inverse of ASCII '@' (xFFC0)
;R4 - Bit counter
;R5 - Increments the address of the histogram 
;R6 - Content of pointer R0

;In this algorithinm, we take the addditive inverse of R3 to get the ACII value of '@' which is then outputted.
;The ASCII of space is loaded and then outputted. The 4 digits that is in the memory
;of the current hstogram address will proceed to be printed. We use a bit counter to copy
;the memory of the current address of the histogram and add it into R2, which is the digit 
;that is created by taking the 4 bits of memory in the current histogram address. 
;Once it has been checked that 4 bits have been added into R2, it goes into LOOP_TWO,
;where it checks for the ASCII value of the digit by subtracting the digit by 9 and then
;adding either the ASCII value of 0 or the ASCII value of A - 10.
;Then, it outputs the digit loops back to EXECLUDE to get the next 4 bits
;so that it can print the next digit. At the start of EXECLUDE, it checks if the 4 digits has been printed by
;subtracting 4 from R1. If 4 digits have been detected, the algorithim goes to the label
;P_NEW_LINE so that it can print a new line and reset the registers. After the 
;start of each line, We check if the current ASCII value is Z so that the 
;histogram stops at the right address, otherwise it repeats.


                AND R4, R4, #0        ;    Clear R4
                AND R5, R5, #0        ;    Clear R5
                AND R1, R1, #0        ;     Clear R1 (Digit Counter) to 0
                

FIRST           AND R1, R1, #0        ;   Clear R1
                AND R0, R0, #0        ;    Clear R0
                NOT R0, R3            ;    Negate R3 into R0
                ADD R0, R0, #1        ;    Add 1 to (R3)'
                ADD R1, R0, #0        ;    Put R0 value into R1 so it can be printed later
                ADD R0, R0, #-15      ;    Subtract 91 (ASCII char after Z)
                ADD R0, R0, #-15      ;
                ADD R0, R0, #-15      ;
                ADD R0, R0, #-15      ;
                ADD R0, R0, #-15      ;
                ADD R0, R0, #-15      ;
                ADD R0, R0, #-1       ;
                BRz DONE              ;    If difference is 0, branch to DONE
                AND R0, R0, #0        ;    Clear R0
                ADD R0, R1, #0        ;    Add R1 into R0
                OUT                   ;    Output Histogram Label
                AND R0, R0, #0        ;     Clear R0
                LD R0, SPACE          ;    Loads space character into R5
                OUT                   ;     Output space
                ADD R3, R3, #-1       ;     Gets next ASCII char in histogram
                BRnzp START           ;     Branch to START
    
START           AND R1, R1, #0        ;    Clear R1
                AND R0, R0, #0        ;     Clear R0    
                LD  R0, HIST_ADDR     ;     Load starting histogram address into R0
                ADD R0, R5, R0        ;     Adding R5 to R0 to increment histogram address    
                LDR R6, R0, #0        ;     Loads content of address in R0 in R6    
            
            
EXCLUDE         AND R0, R0, #0        ;    Clear R0
                ADD R0, R1, #0        ;    Add R1 into R0
                ADD R0, R0, #-4       ;     Subtracting 4 to check if < 4 digits
                BRnp LOOP_ONE         ;    If negative or positive, branch to LOOP_ONE
                BRnzp P_NEW_LINE      ;    Branch to P_NEW_LINE

LOOP_ONE        AND R2, R2, #0        ;    Clears R2
                AND R4, R4, #0        ;    Clears R4

COPY
                AND R0, R0, #0        ;     Clear R0
                ADD R0, R4, #0        ;    Add R4 into R0 
                ADD R0, R0, #-4       ;     Check if the bit counter is 0
                BRz LOOP_TWO          ;    Branch to LOOP_TWO
                ADD R6, R6, #0        ;    Checking for next bit from memory address
                BRn ADD_ONE           ;    Branch to ADD_ONE if negative
                ADD R2, R2, R2        ;   Shift left
                ADD R2, R2, #0        ;    Adding 0 to digit
                BRnzp SHIFT           ;    Branch to SHIFT


ADD_ONE         ADD R2, R2, R2        ;   Shift left digit
                ADD R2, R2, #1        ;    Adds 1 to digit    

SHIFT           ADD R6, R6, R6        ;     Left shift R6
                ADD R4, R4, #1        ;    Increment bit counter
                BRnzp COPY            ;    Branch to COPY        

LOOP_TWO        ADD R4, R4, #0        ;   Clear bit counter
                ADD R2, R2, #-9       ;    Check if digit <= 9 bits
                BRnz ADD_ZERO         ;    If zero or negative, branch to ADD_ZERO 
                ADD R2, R2, #9        ;   Add back 9 to digit
                LD R4, ASCIIA         ;    Load ASCII of 'A' into R4
                ADD R2, R2, #-10      ;    Subtracts 10    
                ADD R2, R4, R2        ;    Adds value of digit (A-10)
                BRzp FINISH           ;    Branch to FINISH

ADD_ZERO        ADD R2, R2, #9        ;   Add back 9 to digit
                AND R4, R4, #0        ;   Reset bit counter
                LD R4, ASCIIO         ;    Loads '0'
                ADD R2, R4, R2        ;    Adds '0' onto digit
FINISH        
                AND R0, R0, #0        ;     Clears R0
                ADD R0, R2, #0        ;     Adds R2 into R0    
                OUT                   ;     Output char    (digit)
                ADD R1, R1, #1        ;     Increments digit counter
                BRnzp EXCLUDE         ;     Branches to EXCLUDE

P_NEW_LINE      AND R0, R0, #0        ;     Clears R0
                LD R0, NEW_LINE       ;     Loads ASCII of new line into R0
                OUT                   ;     Output new line    
                ADD R5, R5, #1        ;     Increment R5
                BRnzp FIRST           ;     Branch to FIRST
            


DONE    HALT            ; done


; the data needed by the program
NUM_BINS    .FILL #27    ; 27 loop iterations
NEG_AT        .FILL xFFC0    ; the additive inverse of ASCII '@'
AT_MIN_Z    .FILL xFFE6    ; the difference between ASCII '@' and 'Z'
AT_MIN_BQ    .FILL xFFE0    ; the difference between ASCII '@' and '`'
HIST_ADDR    .FILL x3F00 ; histogram starting address
STR_START    .FILL x4000    ; string starting address

NEW_LINE     .FILL x000A
SPACE        .FILL x0020
ASCIIO      .FILL x0030
ASCIIA      .FILL x0041


; for testing, you can use the lines below to include the string in this
; program...
; STR_START        .FILL STRING    ; string starting address
; STRING        .STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



    ; the directive below tells the assembler that the program is done
    ; (so do not write any code below it!)

    .END
