#ifndef __NT_DEBOUNCE_H
#define __NT_DEBOUNCE_H

#include <stdint.h>
#include "nt_gpio.h"


typedef enum{
	debounce_EAGER = 0x00,
    debounce_DEFER, 
}nt_debounceType;

typedef enum{
	ONPRESS_CONFIRM = 0x01,
    ONRELEASE_CONFIRM, 		
}nt_triggType;

typedef enum{
	ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1, 		
}nt_activeState;

typedef struct nt_btnArgs{
	nt_PinDefStruct_t* pin;
	nt_debounceType deb_type;	
	uint16_t debounce_time;
	nt_activeState active_state;	
}nt_btnArgs_t;

typedef struct nt_btn{
	uint16_t flags;
	uint8_t state_last;
	uint32_t time_change;
	uint32_t time_change_last;
	
	nt_btnArgs_t* pin_args;
}nt_btn_t;


uint8_t nt_debounceBtn(nt_btn_t* btn, uint32_t mstime);

#endif 