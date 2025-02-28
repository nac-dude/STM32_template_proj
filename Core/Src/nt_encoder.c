#include <stdint.h>

#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"

#include "lwprintf/lwprintf.h"

#include "nt_pins.h"
#include "nt_time.h"
#include "nt_gpio.h"

#include "nt_encoder.h"
#include "nt_debounce.h"

extern nt_PinDefStruct_t pin_encoder1_A;
extern nt_PinDefStruct_t pin_encoder1_B;
extern nt_PinDefStruct_t pin_encoder2_A;
extern nt_PinDefStruct_t pin_encoder2_B;

static uint8_t enc_state; /*these vars are not visible outside of this file(module)*/
static int8_t indent_value; 

static int8_t enc_state_current; 
//static uint8_t state_last; /*declaration of var state_last*/


void nt_EncoderPinInit(void)
{
  nt_gpioPinInit(&pin_encoder1_A, PIN_ENC1_A);
  nt_gpioPinInit(&pin_encoder1_B, PIN_ENC1_B);
  nt_gpioPinInit(&pin_encoder2_A, PIN_ENC2_A);
  nt_gpioPinInit(&pin_encoder2_B, PIN_ENC2_B);
}

uint8_t nt_EncoderPinState(EncoderPinDef_t* encoder)
{
  //tule se uporabi nt_debounceBtn(); namesto nt_readPin()
  uint8_t enc_A = nt_readPin(encoder->btn_enc_A->pin_args->pin);
  uint8_t enc_B = nt_readPin(encoder->btn_enc_B->pin_args->pin);

    return ((enc_B << 1)|enc_A); 
}


int8_t nt_EncoderRead(EncoderPinDef_t* encoder)
{
	enc_state_current = nt_EncoderPinState(encoder);

	if (enc_state_current != encoder -> state_last)
		{
			int8_t dir = 0;
			enc_state = nt_EncoderPinState(encoder);

			switch(encoder -> state_last)
				{
					case 0:
						switch (enc_state)
						{
							case 0: { break; }
							case 1: { dir = +1; break; }
							case 2: { dir = -1; break; }
							case 3: { break; }
						}
						break;
					case 1:
						switch (enc_state)
						{
							case 0: { dir = -1; break; }
							case 1: { break; }
							case 2: { break; }
							case 3: { dir = +1; break; }
						}
						break;
					case 2:
						switch (enc_state)
						{
							case 0: { dir = +1; break; }
							case 1: { break; }
							case 2: { break; }
							case 3: { dir = -1; break; }
						}
						break;
					case 3:
						switch (enc_state)
						{
							case 0: { break; }
							case 1: { dir = -1; break; }
							case 2: { dir = +1; break; }
							case 3: { break; }
						}
						break;
				}


				encoder -> state_last = enc_state;
				indent_value += dir;
				
				if (indent_value >= 4 || indent_value <= -4 )
				{
					int8_t val = indent_value;
					val = (val/4);

					//lwprintf_printf("Inside indent value %d\r\n", val);
					
					indent_value = 0;
					return (val);
				}
		
		}
		return 0;

}
