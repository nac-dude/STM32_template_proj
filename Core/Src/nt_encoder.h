#ifndef __NT_ENCODER_H
#define __NT_ENCODER_H

#include <stdint.h>
#include "nt_debounce.h"

typedef struct
{
    nt_btn_t* btn_enc_A;
    nt_btn_t* btn_enc_B;
    uint8_t state_last;
} EncoderPinDef_t;

void nt_EncoderPinInit();

uint8_t nt_EncoderPinState(EncoderPinDef_t *encoder);

int8_t nt_EncoderRead(EncoderPinDef_t *encoder);

#endif
