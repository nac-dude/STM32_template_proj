
#include "main.h"
#include "lwprintf/lwprintf.h"

#include "nt_usart.h"
#include "nt_pins.h"
#include "nt_gpio.h"

#include "nt_encoder.h"
#include "nt_debounce.h"
#include "nt_LCD_1602.h"

#include "nt_system_init.h"
#include "nt_time.h"
#include "nt_time_utils.h"

void nt_timerCB_CH1(nt_TimerStateEnum state);
void nt_timerCB_CH2(nt_TimerStateEnum state);

uint32_t testMs = 0;

static int lwprintf_my_out_func(int ch, lwprintf_t *p);

nt_PLLDefStruct_t nt_clock_init;

nt_PinDefStruct_t pin_CH1;
nt_PinDefStruct_t pin_CH2;
nt_PinDefStruct_t pin_CH3;

nt_PinDefStruct_t led_nucleo;
nt_PinDefStruct_t pin_btn_nucleo;

nt_PinDefStruct_t pin_encoder1_A;
nt_PinDefStruct_t pin_encoder1_B;
nt_PinDefStruct_t pin_encoder2_A;
nt_PinDefStruct_t pin_encoder2_B;

nt_btnArgs_t arg_pin_nucleo = {&pin_btn_nucleo, debounce_DEFER, 20, ACTIVE_LOW};


nt_btnArgs_t arg_enc1_A = {&pin_encoder1_A, debounce_DEFER, 2, ACTIVE_LOW};
nt_btnArgs_t arg_enc1_B = {&pin_encoder1_B, debounce_DEFER, 2, ACTIVE_LOW};
nt_btnArgs_t arg_enc2_A = {&pin_encoder2_A, debounce_DEFER, 2, ACTIVE_LOW};
nt_btnArgs_t arg_enc2_B = {&pin_encoder2_B, debounce_DEFER, 2, ACTIVE_LOW};

nt_btn_t btn_pin_nucleo = {.pin_args = &arg_pin_nucleo};

nt_btn_t btn_enc1_A = {.pin_args = &arg_enc1_A};
nt_btn_t btn_enc1_B = {.pin_args = &arg_enc1_B};
nt_btn_t btn_enc2_A = {.pin_args = &arg_enc2_A};
nt_btn_t btn_enc2_B = {.pin_args = &arg_enc2_B};

EncoderPinDef_t enc1 = {&btn_enc1_A, &btn_enc1_B, 0};
EncoderPinDef_t enc2 = {&btn_enc2_A, &btn_enc2_B, 0};

nt_TimerDefStruct_t timer_CH1;
nt_TimerDefStruct_t timer_CH2;
nt_TimerDefStruct_t timer_CH3;

bool btn_state, btn_last_state = true;
volatile int i = 1;

int main(void)
{
  bool system_success = nt_systemClock_init(nt_HSE, 8000000, &nt_clock_init);

  nt_gpioPinInit(&led_nucleo, PIN_LED_NUCLEO);
  nt_gpioPinInit(&pin_btn_nucleo, PIN_BTN_NUCLEO);

  nt_gpioPinInit(&pin_CH1, PIN_CH1); // LL_GPIO_PIN_3
  nt_gpioPinInit(&pin_CH2, PIN_CH2); // LL_GPIO_PIN_2
  nt_gpioPinInit(&pin_CH3, PIN_CH3); // LL_GPIO_PIN_1

  nt_EncoderPinInit();
  nt_timeInit();

  nt_usart2_Init();

  // nt_timerInitUs(&timer_CH1, 0, 20000, 25000, nt_timerCB_CH1);
  nt_timerInitUs(&timer_CH2, 0, 30, 100, nt_timerCB_CH2);

  lwprintf_init(lwprintf_my_out_func);

  if (system_success)
  {
    lwprintf_printf("Huston, we have a problem\r\n");
  }

  LL_RCC_ClocksTypeDef freqs;
  LL_RCC_GetSystemClocksFreq(&freqs);
  nt_pinOff(&pin_CH1);

  lwprintf_printf("SYSCLK_Frequency:  %d\r\n", freqs.SYSCLK_Frequency);
  lwprintf_printf("HCLK_Frequency:  %d\r\n", freqs.HCLK_Frequency);
  lwprintf_printf("PCLK1_Frequency:  %d\r\n", freqs.PCLK1_Frequency);
  lwprintf_printf("PCLK2_Frequency:  %d\r\n", freqs.PCLK2_Frequency);

  testMs = nt_timeInitUs32();

  nt_waitUs(15000);

  nt_LCD_Init();

  uint8_t enc_value = 0;
  uint8_t enc_value_old = 0;

  uint16_t enc2_value = 0;
  uint16_t enc2_value_old = 0;

  btn_pin_nucleo.state_last = 1;

  nt_LCD_SetCursor(0, 0);
  lwprintf_printf("SYSCLK: %d\r\n", freqs.SYSCLK_Frequency);

  nt_LCD_Print("Zgoraj");
  nt_LCD_SetCursor(1, 0);
  nt_LCD_Print("Spodaj");

  while (1)
  {
     
      
    if (nt_debounceBtn(&btn_pin_nucleo, nt_timeGetMs32()))
     {
      nt_pinToggle(&led_nucleo);
     }
    
    enc_value += nt_EncoderRead(&enc1);
    
    if (enc_value > enc_value_old)
    {
      nt_LCD_Shift_Right();
      enc_value_old = enc_value;
    }
    else if (enc_value < enc_value_old)
    {
      nt_LCD_Shift_Left();
      enc_value_old = enc_value;
    }
        
    enc2_value += nt_EncoderRead(&enc2);

    if (enc2_value != enc2_value_old)
    {
      if (enc2_value < 30 || enc2_value > 50)
      {
        enc2_value = 30;
      }
      nt_timerInitUs(&timer_CH2, 0, enc2_value, 100, nt_timerCB_CH2);

      enc2_value_old = enc2_value;
    }
    
    nt_timerUs(&timer_CH2);
  }
}

void nt_timerCB_CH1(nt_TimerStateEnum state)
{
  switch (state)
  {
  case stateOff:
  {
    nt_pinSet(&pin_CH1, false);
    break;
  }

  case stateOn:
  {
    nt_pinSet(&pin_CH1, true);
    break;
  }

  case statePre:
  {
    nt_pinSet(&pin_CH1, true);
    break;
  }

  default:
    break;
  }
}

void nt_timerCB_CH2(nt_TimerStateEnum state)
{
  switch (state)
  {
  case stateOff:
  {
    nt_pinSet(&pin_CH2, false);
    break;
  }

  case stateOn:
  {
    nt_pinSet(&pin_CH2, true);
    break;
  }

  default:
    break;
  }
}

static int lwprintf_my_out_func(int ch, lwprintf_t *p)
{
  uint8_t c = (uint8_t)ch;

  /* Don't print zero */
  if (c == '\0')
  {
    return ch;
  }
  // nt_uart2_Transmitt8(c);
  nt_uart2_Transmitt8(c);
  return ch;
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
