#include "nt_debounce.h"
#include <stdbool.h>
#include "lwprintf/lwprintf.h"

uint8_t nt_debounceBtn(nt_btn_t *btn, uint32_t mstime)
{
  uint8_t current_state;

  current_state = nt_readPin(btn->pin_args->pin);

  // DEFER
  if (current_state != btn->state_last)
  {
    btn->time_change_last = mstime;
  }
  else if (current_state == btn->pin_args->active_state)
  {
    if (!(btn->flags & ONPRESS_CONFIRM))
    {
      if (mstime - btn->time_change_last >= btn->pin_args->debounce_time)
      {
        btn->flags |= ONPRESS_CONFIRM;
        btn->state_last = current_state;
        return true;
      }
    }
  }
  else
  {
    if (btn->flags & ONPRESS_CONFIRM)
    {
      btn->flags &= ~(ONPRESS_CONFIRM);
    }
  }
  btn->state_last = current_state;
  return false;
}
