
#ifndef __NT_TIME_H
#define __NT_TIME_H

//#define USE_INTERRUPT_TIM1
//#define USE_INTERRUPT_MILLISECONDS_FUNC

void TIM1_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void nt_timeInit(void);

#ifdef USE_INTERRUPT_MILLISECONDS_FUNC
void generate_milliseconds(void);
#endif

uint32_t nt_timeGetUs32();
uint32_t nt_timeInitUs32();
uint32_t nt_timeGetMs32();

#endif