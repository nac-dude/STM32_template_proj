
#include <string.h>

#include "stm32f1xx_ll_gpio.h"
#include "nt_time_utils.h"
#include "nt_pins.h"
#include "nt_gpio.h"

#include "nt_LCD_1602.h"

static nt_PinDefStruct_t pin_RS;
static nt_PinDefStruct_t pin_RW;
static nt_PinDefStruct_t pin_Enable;
static nt_PinDefStruct_t pin_DB7;
static nt_PinDefStruct_t pin_DB6;
static nt_PinDefStruct_t pin_DB5;
static nt_PinDefStruct_t pin_DB4;

void nt_LCD_pinInit()
{
    nt_gpioPinInit(&pin_RS, PIN_LCD_RS);
    nt_gpioPinInit(&pin_RW, PIN_LCD_RW);
    nt_gpioPinInit(&pin_Enable, PIN_LCD_E);
    nt_gpioPinInit(&pin_DB7, PIN_LCD_DB7);
    nt_gpioPinInit(&pin_DB6, PIN_LCD_DB6);
    nt_gpioPinInit(&pin_DB5, PIN_LCD_DB5);
    nt_gpioPinInit(&pin_DB4, PIN_LCD_DB4);
}

void nt_LCD_SendData(const uint8_t *data, uint8_t RS) // RS = 1 data ; RS = 0 command
{
    // hi-bits first
    const uint8_t DB7 = (*data >> 3) & 1;
    const uint8_t DB6 = (*data >> 2) & 1;
    const uint8_t DB5 = (*data >> 1) & 1;
    const uint8_t DB4 = (*data >> 0) & 1;

    nt_pinSet(&pin_RS, RS);
    nt_pinSet(&pin_RW, false); // false = write ; true = read

    nt_pinSet(&pin_DB7, DB7);
    nt_pinSet(&pin_DB6, DB6);
    nt_pinSet(&pin_DB5, DB5);
    nt_pinSet(&pin_DB4, DB4);

    nt_pinSet(&pin_Enable, true);
    nt_waitUs(LCD_TIME_LATCH_DATA);
    nt_pinSet(&pin_Enable, false);
    nt_waitUs(LCD_TIME_LATCH_DATA);
}

void nt_LCD_SendInstruction(LCD_instruction instruction)
{
    const uint8_t data_hi = ((instruction >> 4) & 0x0f);
    const uint8_t data_lo = ((instruction) & 0x0f);

    nt_LCD_SendData(&data_hi, 0);
    nt_LCD_SendData(&data_lo, 0);
    nt_waitUs(LCD_TIME_PROCESS_DATA);
}

void nt_LCD_Init(void)
{
    nt_LCD_pinInit();
    nt_LCD_SendInstruction(function_set);
    nt_LCD_SendInstruction(entry_mode_set);
    nt_LCD_SendInstruction(display_on);
    nt_LCD_SendInstruction(cursor_off);
    nt_LCD_SendInstruction(clear_display);
    nt_LCD_SendInstruction(return_home);
    nt_waitUs(1550);
    nt_LCD_SendInstruction(set_DDRAM_address_line_00);
}

void nt_LCD_SetCursor(uint8_t line, uint8_t pos)
{
    switch (line)
    {
    case 0:
        nt_LCD_SendInstruction(set_DDRAM_address_line_00 + pos);
        break;

    case 1:
        nt_LCD_SendInstruction(set_DDRAM_address_line_10 + pos);
        break;

    default:
        break;
    }
}

void nt_LCD_Shift_Left(void)
{
    nt_LCD_SendInstruction(display_shift_left);
}

void nt_LCD_Shift_Right(void)
{
    nt_LCD_SendInstruction(display_shift_right);
}


void nt_LCD_PrintChar(const char c)
{
    uint8_t print_chr = ((c >> 4) & 0x0f);
    nt_LCD_SendData(&print_chr, 1);

    print_chr = ((c) & 0x0f);
    nt_LCD_SendData(&print_chr, 1);
    nt_waitUs(LCD_TIME_PROCESS_DATA);
}


void nt_LCD_PrintLen(const char *str, int len)
{
    int i = 0;
    while (i < len)
    {
        const char c = str[i];
        if (!c)
            break;

        nt_LCD_PrintChar(c);

        i++;
    }
}

void nt_LCD_Print(const char *str)
{
    nt_LCD_PrintLen(str, 24);
}

