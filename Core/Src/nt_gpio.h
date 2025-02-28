#ifndef __NT_GPIO_H
#define __NT_GPIO_H

#include <stdint.h>
#include <stdbool.h>


//Pin type
//nt_Unknown
//nt_Input
//nt_Output
typedef enum
{
  nt_Unknown,
  nt_Input,
  nt_Output
} nt_PinTypeEnum_t;

typedef struct
{
  uint32_t port;
  uint32_t pin;
  uint32_t mode;
  nt_PinTypeEnum_t pin_type;
} nt_PinDefStruct_t;

void nt_gpioPinInit(nt_PinDefStruct_t *pin_def, uint32_t port, uint32_t pin, uint32_t mode, nt_PinTypeEnum_t pin_type);

void nt_pinOn(nt_PinDefStruct_t *pin_def);
void nt_pinOff(nt_PinDefStruct_t *pin_def);
void nt_pinToggle(nt_PinDefStruct_t *pin_def);
void nt_pinSet(nt_PinDefStruct_t *pin, bool state);

bool nt_readPin(nt_PinDefStruct_t *pin_def);

#endif