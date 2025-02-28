
#ifndef __NT_SYSTEM_INIT_H
#define __NT_SYSTEM_INIT_H

#include <stdint.h>
#include <stdbool.h>


typedef enum 
{
	nt_HSE, 
	nt_HSI,
}nt_PLLSourceEnum_t;

typedef struct
{
    uint32_t nt_HSE_freq;
    uint32_t nt_pll_mul;
    uint32_t nt_pll_prediv;
    uint32_t nt_AHB_prescaler;
    uint32_t nt_APB1_prescaler;
    uint32_t nt_APB2_prescaler;
} nt_PLLDefStruct_t;


//returns true if error
bool nt_systemClock_init(nt_PLLSourceEnum_t nt_pll_source, uint32_t hse_clk_freq, nt_PLLDefStruct_t *nt_pll_def);

/*
  .word HardFault_Handler
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
*/

#endif