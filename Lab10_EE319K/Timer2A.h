#ifndef __TIMER2INTS_H__ // do not include more than once
#define __TIMER2INTS_H__
#include <stdint.h>
// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  period in units (1/clockfreq)
//          priority is 0 (high) to 7 (low)
// Outputs: none
void Timer2A_Init(uint32_t period);
#endif // __TIMER2INTS_H__