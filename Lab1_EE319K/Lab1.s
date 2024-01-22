;****************** main.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Place your name here
; Date Created: 1/15/2018 
; Last Modified: 12/30/2022 
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1
GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       PRESERVE8 
       AREA   Data, ALIGN=2
; Declare global variables here if needed
; with the SPACE assembly directive
       Z SPACE 1
		THUMB
       EXPORT EID
EID    DCB "aew3364",0  ;replace ABC123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 1 ; change to nonzero when ready for grading
           
      EXPORT  Lab1

Lab1	
	LDR R0, =SYSCTL_RCGCGPIO_R
	LDR R1, [R0]
	
	ORR R1, #0x10
	STR R1, [R0]
	
	; wait two clock cycles
	NOP
	NOP
	
	; Set direction bits for each pin
	LDR R0, =GPIO_PORTE_DIR_R 
	LDR R1,[R0]
	
	BIC R1, #0x07 ; PE0 & PE1 & PE2 pins are set to input
	ORR R1, #0x20 ; PE5 is set to output bit
	STR R1, [R0]
	; Digitally enable all pins
	LDR R0, =GPIO_PORTE_DEN_R
	LDR R1, [R0]
	
	ORR R1, #0x27
	STR R1, [R0]
	
	LDR R0, =GPIO_PORTE_DATA_R ;

loop
       LDR R0,=Z
			MOV R1,#42
			STRB R1,[R0]
b loop

    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
               