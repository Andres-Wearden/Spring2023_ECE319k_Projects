// Lab5.c starter program EE319K Lab 5, Spring 2023
// Runs on TM4C123
// Put your names here: Anika Sridhar; Andres Wearden

// 2/5/2023 version


/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south facing red light connected to bit 2
// north/south facing yellow light connected to bit 1
// north/south facing green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "aew3364"; //  ;replace abc123 with your EID
char EID2[] = "as225242"; //  ;replace abc123 with your EID

#define goS 0
#define waitS 1
#define allStopSW 2
#define goW 3
#define waitW 4
#define allStopWS 5
#define goK 6
#define waitK 7
#define ledOff 8
#define ledOn 9
#define ledOff_2 10
#define totalStop 11


extern void Wait10ms(uint32_t);

void DisableInterrupts(void);
void EnableInterrupts(void);
uint32_t CS = goS;
int main(void){ 
	volatile uint32_t delay;
  DisableInterrupts();
  TExaS_Init(GRADER);
	//Clock Init
	SYSCTL_RCGCGPIO_R |= 0x31;
	delay = 100;
	
	
	//Direction set
	GPIO_PORTA_DIR_R &=~0x38;
	GPIO_PORTE_DIR_R |= 0x3F;
	GPIO_PORTF_DIR_R |= 0x0E;
	
	//Enabling Pins
	GPIO_PORTA_DEN_R |=0x38;
	GPIO_PORTE_DEN_R |=0x3F;
	GPIO_PORTF_DEN_R |=0x0E;
	
	
  
	struct State{
		uint32_t out0;
		uint32_t out;
		uint32_t time;
		uint32_t Next[8];
		};
	typedef const struct State State_t;
		
		State_t FSM[12] = {{0x01, 0x21 , 30 , {goS, waitS, goS, waitS, waitS, waitS, waitS, waitS}}, 
												{0x01, 0x22 , 5 , {allStopSW, allStopSW, allStopSW, allStopSW, allStopSW, allStopSW, allStopSW, allStopSW}},
												{0x01, 0x24 , 1 , {allStopSW, goW, goS, goW, goK, goW, goK, goW}}, 
												{0x01, 0x0C , 30 , {goW, goW, waitW, waitW, waitW, waitW, waitW, waitW}}, 
												{0x01, 0x14 , 5 , {allStopWS, allStopWS, allStopWS, allStopWS, allStopWS, allStopWS, allStopWS, allStopWS}}, 
												{0x01, 0x24 , 1 , {allStopWS, goW, goS, goS, goK, goK, goK, goK}},
												{0x07, 0x24 , 30 , {goK, goK, waitK, waitK, goK, waitK, waitK, waitK}}, 
												{0x01, 0x24 , 1 , {ledOff, ledOff, ledOff, ledOff, ledOff, ledOff, ledOff, ledOff}},
												{0x00, 0x24 , 1 , {ledOn, ledOn, ledOn, ledOn, ledOn, ledOn, ledOn, ledOn}}, 
												{0x01, 0x24 , 1 , {ledOff_2, ledOff_2, ledOff_2, ledOff_2, ledOff_2, ledOff_2, ledOff_2, ledOff_2}}, 
												{0x00, 0x24 , 1 , {totalStop, totalStop, totalStop, totalStop, totalStop, totalStop, totalStop, totalStop}}, 
												{0x01, 0x24 , 1 , {totalStop, goW, goS, goS, goK, goW, goS, goS}}};									
  EnableInterrupts(); // grader, scope, logic analyzer need interrupts
  while(1){
		GPIO_PORTE_DATA_R = FSM[CS].out&0x3F;
		GPIO_PORTF_DATA_R = (FSM[CS].out0&0x07)*2;
		Wait10ms(FSM[CS].time);
		uint32_t in = (GPIO_PORTA_DATA_R&0x38)/8;
		CS = FSM[CS].Next[in];
		}
}



