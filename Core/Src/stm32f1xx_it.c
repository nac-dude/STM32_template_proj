#include "main.h"
#include "stm32f1xx_it.h"

#include "nt_time.h"




/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
    _1_slow_10_fast_blink();
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
    /* USER CODE BEGIN LL_EXTI_LINE_13 */

    /* USER CODE END LL_EXTI_LINE_13 */
  }
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

void TIM1_UP_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM1))
  {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    
    #ifdef USE_INTERRUPT_MILLISECONDS_FUNC
    generate_milliseconds();
    #endif

  }
}

void wait_short(void)
{

  volatile int i;

  i = 100000;
  while (i)
  {
    i--;
  }
  return;
}

void wait_long(void)
{

  volatile int i;

  i = 1000000;
  while (i)
  {
    i--;
  }
  return;
}

void _1_slow_10_fast_blink(void)
{

  volatile int k = 1;
  volatile int j = 10;

  while (k)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    k--;
  }

  while (j)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    j--;
  }
  return;
}

void _2_slow_10_fast_blink(void)
{

  volatile int k = 2;
  volatile int j = 10;

  while (k)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    k--;
  }

  while (j)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    j--;
  }
  return;
}

void _3_slow_10_fast_blink(void)
{

  volatile int k = 3;
  volatile int j = 10;

  while (k)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    k--;
  }

  while (j)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    j--;
  }
  return;
}

void _4_slow_10_fast_blink(void)
{

  volatile int k = 4;
  volatile int j = 10;

  while (k)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_long();
    k--;
  }

  while (j)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    wait_short();
    j--;
  }
  return;
}
