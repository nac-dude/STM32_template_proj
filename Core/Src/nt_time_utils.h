
#ifndef __NT_TIME_UTILS_H
#define __NT_TIME_UTILS_H

#include <stdbool.h>
#include <stdint.h>


typedef enum 
{
  statePre,
  stateOn, 
  stateOff,
  
}nt_TimerStateEnum;

typedef void(*nt_timer_cb_fn)(nt_TimerStateEnum);

typedef struct 
{
  uint32_t timestamp;
  
  uint32_t time_On;  
  uint32_t time_Off; 
  uint32_t time_Max; 

  nt_TimerStateEnum state;
  uint32_t stateTime;

  nt_timer_cb_fn timer_callback_fn; // Pointer to call-back function

} nt_TimerDefStruct_t; //preimenuj 


void nt_waitUs(uint32_t Us);
bool nt_delayCheckUs(uint32_t *last, uint32_t delay);
bool nt_delayCheckUs_noUpdate(uint32_t last, uint32_t delay);
void nt_timerInitUs(nt_TimerDefStruct_t *timer_def, uint32_t t0, uint32_t t1, uint32_t t2, nt_timer_cb_fn asymm_CB_fn);
void nt_timerUs(nt_TimerDefStruct_t *timer_def);

#endif


/*
void nt_timeToggleInit(nt_TimeToggleDefStruct_t *toggle_def, uint16_t phase_shift, uint16_t duty_cycle, uint16_t freq, nt_asymm_cb_fn asymm_CB_fn)
{ 
  toggle_def->time = nt_timeInit();
  
  //T_freq/Tclk = (1/1000)/(1/1000000) = 1000
  toggle_def->timeMax = 1000;

  //phase = 0-360 deg
  uint8_t phase = (phase_shift/360)*toggle_def->timeMax;
  toggle_def->timeOn = phase;

  //duty_cycle = 0-100 %
  toggle_def->timeOff = (toggle_def->timeOn + (toggle_def->timeMax * (duty_cycle*0.01)));
  
  toggle_def->state = stateOn;
  toggle_def->stateTime = toggle_def->timeOn;
  toggle_def->time_callback_fn = asymm_CB_fn;
}
*/


