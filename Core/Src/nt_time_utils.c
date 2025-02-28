
#include <stdint.h>
#include <stdbool.h>

#include "stm32f1xx_ll_gpio.h"

#include "lwprintf/lwprintf.h"

#include "nt_gpio.h"
#include "nt_time.h"
#include "nt_time_utils.h"

void nt_waitUs(uint32_t Us)
{
  volatile int i = (Us * 7);
  while (i--)
  {
  }
  return;
}

bool nt_delayCheckUs(uint32_t *last, uint32_t delay)
{
  uint32_t time = nt_timeGetUs32();
  if (time >= *last)
  {
    if ((time - *last) >= delay)
    {
      *last = time;
      return true;
    }
  }
  else
  {
    if ((0xFFFFFFFF - (*last - time)) >= delay)
    {
      *last = time;
      return true;
    }
  }
  return false;
}

bool nt_delayCheckUs_noUpdate(uint32_t last, uint32_t delay)
{
  uint32_t time = nt_timeGetUs32();
  if (time >= last)
  {
    if ((time - last) >= delay)
    {
      return true;
    }
  }
  else
  {
    if ((0xFFFFFFFF - (last - time)) >= delay)
    {
      return true;
    }
  } 
  return false;
}

void nt_timerInitUs(nt_TimerDefStruct_t *toggle_def, uint32_t t0, uint32_t t1, uint32_t t2, nt_timer_cb_fn asymm_CB_fn)
{
  if (t0 < t1 && t1 < t2)
  {
    toggle_def->timestamp = nt_timeInitUs32();
    toggle_def->time_On = t0;
    toggle_def->time_Off = t1;
    toggle_def->time_Max = t2;

    toggle_def->timer_callback_fn = asymm_CB_fn;

    toggle_def->state = stateOff;
    toggle_def->stateTime = 0;
  }
}

void nt_timerUs(nt_TimerDefStruct_t *toggle_def)
{

  if (nt_delayCheckUs_noUpdate((toggle_def->timestamp), toggle_def->stateTime))
  {
    switch (toggle_def->state)
    {
    case statePre:
    {
      toggle_def->state = stateOn;
      toggle_def->stateTime = toggle_def->time_Off;
      break;
    }

    case stateOn:
    {
      toggle_def->state = stateOff;
      toggle_def->stateTime = toggle_def->time_Max;
      break;
    }

    case stateOff:
    {
      toggle_def->state = statePre;
      toggle_def->stateTime = toggle_def->time_On;
      toggle_def->timestamp = nt_timeGetUs32();
      break;
    }

    default:
      break;
    }

    toggle_def->timer_callback_fn(toggle_def->state);
  }
}

/*
bool nt_delayCheckMs(uint32_t *last, uint32_t delay)
{
  uint32_t time = nt_timeGetUs32();
  if (time >= *last)
  {
    if ((time - *last) >= delay)
    {
      *last = time;
      return true;
    }
  }
  else
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_3);
    if ((0xFFFFFFFF - (*last - time)) >= delay)
    {
      *last = time;
      return true;
    }
  }
  return false;
}
*/

/*
bool nt_delayCheckUs_noUpdate(uint32_t last, uint32_t delay)
{
  uint32_t time = nt_timeGetUs32();
  if (time >= last)
  {
    if ((time - last) >= delay)
    {
      return true;
    }
  }
  else
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_3);
    if ((0xFFFFFFFF - (last - time)) >= delay)
    {
      return true;
    }
  }
  return false;
}
*/

/*
{
  uint32_t time = nt_timeGetUs32();
  if ((time - *last) >= delay)
    {
      *last = time;
      return true;
    }
  else
   return false;
}

*/