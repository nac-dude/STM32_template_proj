
#include <stdint.h>

#include "stm32f103xb.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"

#include "nt_time_utils.h"
#include "nt_gpio.h"

void nt_gpioPinInit(nt_PinDefStruct_t *pin_def, uint32_t port, uint32_t pin, uint32_t mode, nt_PinTypeEnum_t pin_type)
{
    //naredi case ki vklopijo uro periferije preko Port var-a
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);

    //LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    
    pin_def->port = port;
    pin_def->pin = pin;

    LL_GPIO_InitTypeDef gpio_init;

    gpio_init.Pin = pin_def->pin;
    gpio_init.Mode = mode;

    switch (pin_type)
    {
    case nt_Output:
        //preveri če je output as push-pull ali open drain in na podlagi tega določiš tudi pull-up        
        gpio_init.Speed = LL_GPIO_SPEED_FREQ_LOW;
        gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        gpio_init.Pull = LL_GPIO_PULL_DOWN;
        
        //gpio_init.Pull = LL_GPIO_PULL_UP;
        //gpio_init.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
        break;

    case nt_Input:
        gpio_init.Speed = LL_GPIO_SPEED_FREQ_LOW;
        gpio_init.Pull = LL_GPIO_PULL_UP;
        break;

    default:
        break;
    }

    LL_GPIO_Init((GPIO_TypeDef *)pin_def->port, &gpio_init);
}


void nt_pinOn(nt_PinDefStruct_t *pin_def)
{
    LL_GPIO_SetOutputPin((GPIO_TypeDef *)pin_def->port, pin_def->pin);
}


void nt_pinOff(nt_PinDefStruct_t *pin_def)
{
    LL_GPIO_ResetOutputPin((GPIO_TypeDef *)pin_def->port, pin_def->pin);
}


void nt_pinToggle(nt_PinDefStruct_t *pin_def)
{
    LL_GPIO_TogglePin((GPIO_TypeDef *)pin_def->port, pin_def->pin);
}


void nt_pinSet(nt_PinDefStruct_t *pin, bool state)
{
    if (state)
    { 
        nt_pinOn(pin);
        return;
    }
    nt_pinOff(pin);
}



bool nt_readPin(nt_PinDefStruct_t *pin_def){
    bool value = LL_GPIO_IsInputPinSet((GPIO_TypeDef *)pin_def->port, pin_def->pin);
    return value;
}

