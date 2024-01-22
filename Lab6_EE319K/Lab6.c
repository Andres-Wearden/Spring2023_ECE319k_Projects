// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: Anika Shridhar; Andres Wearden
// Date Created: 3/6/17 
// Last Modified: 1/2/23  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Song.h"
#include "../inc/wave.h"
#include "Lab6Grader.h"
#include "Sound.h"
#define C7 4800
#define E0 3760
#define G0 3200
#define A0 2880
// put both EIDs in the next two lines
char EID1[] = "aew3364"; //  ;replace abc123 with your EID
char EID2[] = "as225242"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint32_t data);   // your lab 6 solution
void WaitForInterrupt();
uint8_t Testdata;

// lab video Lab6_voltmeter
// use this if you have a voltmeter
// 1) connect voltmeter to DACOUT 
// 2) add a breakpoint at i+1
// 3) run and record voltage for each input value
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};

int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}
// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static
// use this if you DO NOT have a voltmeter
// you need to implement LaunchPad_Init and LaunchPad_Input
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}



int main(void){       
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
//    SCOPE,
//    LOGICANALYZERA,
//    LOGICANALYZERB,
//    LOGICANALYZERC,
//    LOGICANALYZERE,
//    REALBOARDGRADER,
//    SIMULATIONGRADER,
//    NONE
  Key_Init();
  LaunchPad_Init(); // Initializes PORTF with proper output bits enabled
  Sound_Init(); //initializes the sound wave output (Systic and DAC)
  //Song_Init(); // extra credit 1)
  //Wave_Init(); // extra credit 2)
  EnableInterrupts();
	//const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
  while(1){
		uint32_t j = 0;
	if(GPIO_PORTE_DATA_R == 0x01){
				Sound_Start(C7); //takes an input period in "tick" units and will play sound wave at that frequency
				WaitForInterrupt();
		}
			else if(GPIO_PORTE_DATA_R == 0x02){
				Sound_Start(E0);
				WaitForInterrupt();
		}
			else if(GPIO_PORTE_DATA_R == 0x04){
				Sound_Start(G0);
				WaitForInterrupt();
		}
			else if(GPIO_PORTE_DATA_R == 0x08){
				Sound_Start(A0);
				WaitForInterrupt();
		}
			/*else if(GPIO_PORTE_DATA_R == 0x0A){
				Sound_Start(7000);
				WaitForInterrupt();
		}*/
			else if((GPIO_PORTE_DATA_R == 0x03) || (GPIO_PORTE_DATA_R == 0x05) || (GPIO_PORTE_DATA_R == 0x09) || (GPIO_PORTE_DATA_R == 0x06)){
				Sound_Start(A0);
				WaitForInterrupt();
			}
			else{
			Sound_Off();
			GPIO_PORTF_DATA_R ^= 0x02;
			DelayMs(100);
			};
		}
	}
