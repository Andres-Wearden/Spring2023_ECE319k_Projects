#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

//void DisableInterrupts(void); // Disable interrupts
//void EnableInterrupts(void);  // Enable interrupts
//long StartCritical (void);    // previous I bit, disable interrupts
//void EndCritical(long sr);    // restore I bit to previous value
//void WaitForInterrupt(void);  // low power mode

void PortE_Init(void){    
	volatile long delay;	
  SYSCTL_RCGCGPIO_R |= 0x00000010; // (a) activate clock for port E
	while((SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x0E; // (c) make PE3 in (built-in button)
	GPIO_PORTE_DEN_R |= 0x0E; // enable digital I/O on PE3
	GPIO_PORTE_PDR_R |= 0x0E;
	GPIO_PORTE_IS_R &= ~0x0E;     // (d) PE3 is edge-sensitive
	GPIO_PORTE_IBE_R &= ~0x0E;
	GPIO_PORTE_IEV_R |= 0x0E;
	GPIO_PORTE_ICR_R = 0x0E;      // (e) clear flag
  GPIO_PORTE_IM_R |= 0x0E;      // (f) arm interrupt on PF4
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000080; // (g) priority 5
	NVIC_EN0_R = 0x10;      // (h) enable interrupt 29 in NVIC
}

