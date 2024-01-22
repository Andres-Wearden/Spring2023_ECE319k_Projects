#ifndef __Enemy_H__ // do not include more than once
#define __Enemy_H__
#include <stdint.h>
// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  period in units (1/clockfreq)
//          priority is 0 (high) to 7 (low)
// Outputs: none
extern void Timer0_Init(uint32_t period, uint32_t priority);
#endif // __TIMER2INTS_H__