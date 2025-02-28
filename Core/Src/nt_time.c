
#include <stdint.h>

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"

#include "lwprintf/lwprintf.h"

#include "nt_gpio.h"
#include "nt_time.h"

#ifdef USE_INTERRUPT_MILLISECONDS_FUNC
static uint32_t milliseconds = 0;
#endif

extern nt_PinDefStruct_t pin_CH1;

void TIM1_Init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

// Prescaler: 64000 -> CNT updates at 1kHz; 1ms -> no update event
#ifndef USE_INTERRUPT_TIM1
  TIM_InitStruct.Prescaler = 63;
  TIM_InitStruct.Autoreload = 0xFFFF;
#endif

#ifdef USE_INTERRUPT_TIM1
  TIM_InitStruct.Prescaler = 64;
  TIM_InitStruct.Autoreload = 999;
#endif

  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);

  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);

  LL_TIM_EnableCounter(TIM1);

#ifdef USE_INTERRUPT_TIM1
  LL_TIM_EnableIT_UPDATE(TIM1);
  NVIC_SetPriority(TIM1_UP_IRQn, 0);
  NVIC_EnableIRQ(TIM1_UP_IRQn);
#endif
}

void TIM2_Init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_TIM_InitTypeDef TIM2_InitStruct = {0};

#ifndef USE_INTERRUPT_TIM2
  TIM2_InitStruct.Prescaler = 63;
  TIM2_InitStruct.Autoreload = 0xFFFF;
#endif

#ifdef USE_INTERRUPT_TIM1
  TIM_InitStruct.Prescaler = 64;
  TIM_InitStruct.Autoreload = 999;
#endif

  TIM2_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM2_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM2_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM2, &TIM2_InitStruct);

  // LL_TIM_SetCounter(TIM3, 0xffff);

  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_EnableUpdateEvent(TIM2);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
  LL_TIM_EnableCounter(TIM2);

#ifdef USE_INTERRUPT_TIM1
  LL_TIM_EnableIT_UPDATE(TIM1);
  NVIC_SetPriority(TIM1_UP_IRQn, 0);
  NVIC_EnableIRQ(TIM1_UP_IRQn);
#endif
}

void TIM3_Init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  LL_TIM_InitTypeDef TIM3_InitStruct = {0};

#ifndef USE_INTERRUPT_TIM3
  TIM3_InitStruct.Prescaler = 0;
  TIM3_InitStruct.Autoreload = 0xFFFF;
#endif

  TIM3_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM3_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM3_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM3, &TIM3_InitStruct);

  // LL_TIM_SetCounter(TIM3, 0xffff);

  LL_TIM_SetSlaveMode(TIM3, LL_TIM_SLAVEMODE_TRIGGER);
  LL_TIM_SetTriggerInput(TIM3, LL_TIM_TS_ITR1);
  LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_EXT_MODE1);
  LL_TIM_EnableCounter(TIM3);
}

void nt_timeInit()
{
  // TIM1_Init();
  TIM3_Init();
  TIM2_Init();
}

uint32_t nt_timeGetUs32()
{
  uint32_t time_1 = (LL_TIM_GetCounter(TIM3) << 16) | LL_TIM_GetCounter(TIM2);

  uint32_t time_2 = (LL_TIM_GetCounter(TIM3) << 16) | LL_TIM_GetCounter(TIM2);

  if (time_2 > time_1)
  {
    return time_2;
  }
  return time_1;
}

uint32_t nt_timeInitUs32()
{
#if (0)
  return nt_timeGetMs16();
#else
  return nt_timeGetUs32();
#endif
}


uint32_t nt_timeGetMs32()
{
 return (nt_timeGetUs32() >> 10);
}


#ifdef USE_INTERRUPT_MILLISECONDS_FUNC
void generate_milliseconds(void)
{
  milliseconds++;
}
#endif

#ifdef USE_INTERRUPT_MILLISECONDS_FUNC
uint32_t nt_timeGetMs32()
{
  return milliseconds;
}
#endif

// Microsec clock
/*
if (time_lobyte & 1)
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_2);
*/
/*
  //Lobyte 0xFFFF us = 65,5ms
  if (time_lobyte < time_last)
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_3); //CH1 - Rumen
  time_last = time_lobyte;

  //Hibyte clock period = 65,5ms
  if (time_hibyte & 1)
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_2); //CH2 - Moder
  else
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);

  if (time_hibyte < time_last_32)
    LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_4); //CH3 - vijoličen
  time_last_32 = time_hibyte;
*/

/*
uint32_t nt_timeGetMs32()
{
 return (nt_timeGetUs32() >> 10);
}
*/

/*
#ifndef USE_INTERRUPT_MILLISECONDS_FUNC
  uint32_t nt_timeGetMs32()
  {
    static uint16_t time_lobyte = 0;
    static uint16_t time_hibyte = 0;

    uint16_t time = nt_timeGetMs16();
    if (time < time_lobyte)
    {
      time_hibyte++;
      LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_2);
    }
    time_lobyte = time;
    return (time_hibyte << 16) | time_lobyte;
  }
#endif
*/

/*
static uint32_t last = 0;

uint32_t result = (LL_TIM_GetCounter(TIM3) << 16) | LL_TIM_GetCounter(TIM2);

if (result - last > (20 << 16))
{
  lwprintf_printf("res: %x, %x  \r\n", result, last);
  if (result < last)
  {
    lwprintf_printf("res: %u  \r\n", last - result);
  }

  last = result;
}

return result;
*/