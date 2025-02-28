#ifndef __NT_LCD_1602_H
#define __NT_LCD_1602_H

/*

RS and R/W -> excluded in the instruction byte since both are hardwired in the SendInstruction() and SendString() functions

---------------------------------------------------------

FUNCTION SET: 4bit data length, 2 line display, 5x8 dots

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   1   0   1   0   0   0

HEX: 0x28

---------------------------------------------------------

CLEAR DISPLAY:

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   0   0   0   1

HEX: 0x01

---------------------------------------------------------

RETURN HOME:

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   0   0   1   0

HEX: 0x02

---------------------------------------------------------

DISPLAY ON: display on, cursor on, no cursor blinking

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   1   1   1   0

HAX: 0x0E

---------------------------------------------------------

DISPLAY OFF: display off

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   1   0   1   0

HAX: 0x0A

---------------------------------------------------------

CURSOR_OFF: display on, cursor off, no cursor blinking

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   1   1   0   0

HAX: 0x0C

---------------------------------------------------------

CURSOR_ON: display on, cursor on, no cursor blinking

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   1   1   1   0

HAX: 0x0D

---------------------------------------------------------

ENTRY MODE SET: increment DDRAM address, no display shift

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   0   0   1   1   0

HEX: 0x06

---------------------------------------------------------

CURSOR SHIFT: cursor shift to right

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   1   0   1   0   0

HEX: 0x14

-------------------------------------------------------

DISPLAY_SHIFT_LEFT: display shift to left

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   1   1   0   0   0

HEX: 0x18

-------------------------------------------------------

DISPLAY_SHIFT_RIGHT: display shift to right

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   0   0   0   1   1   1   0   0

HEX: 0x1C

-------------------------------------------------------

SET DDRAM address:

RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
0  0   1   1   0   0   0   0   0   0

HEX: 0x40

-------------------------------------------------------

#define FUNCTION_SET 0x28 // 4bit data length, 2 line display, 5x8 dots
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02 // return cusrsor home
#define DISPLAY_ON 0x0E
#define DISPLAY_OFF 0x0A
#define ENTRY_MODE_SET 0x06 // increment DDRAM address, no display shift
#define CURSOR_SHIFT 0x14   // cursor shifts right
*/

#define LCD_TIME_PROCESS_DATA 20 //micro seconds
#define LCD_TIME_LATCH_DATA   5  //micro seconds

typedef enum
{
    function_set         = 0x28,       //4bit data length, 2 line display, 5x8 dots
    entry_mode_set       = 0x06,       //increment DDRAM address, auto cursor move to right, no display shift

    clear_display        = 0x01,
    return_home          = 0x02,       //return cursor home
    
    display_on           = 0x08,
    display_off          = 0x0A,

    cursor_on            = 0x0E,
    cursor_off           = 0x0C,
        
    display_shift_left   = 0x18,
    display_shift_right  = 0x1C,
    
    cursor_shift_right   = 0x14, //cursor shifts right
    cursor_shift_left    = 0x10,  //cursor shifts left
    
    set_DDRAM_address_line_00   = 0x80,   //set DDRAM address -> 0x80 first position of first line 
    set_DDRAM_address_line_10   = 0xC0     //set DDRAM address -> 0x40 first position of second line 

} LCD_instruction;

void nt_LCD_pinInit(void);

void nt_LCD_SendData(const uint8_t *data, uint8_t RS);
void nt_LCD_SendInstruction(LCD_instruction instruction);

void nt_LCD_SetCursor(uint8_t line, uint8_t pos);
void nt_LCD_Shift_Left(void);
void nt_LCD_Shift_Right(void);

void nt_LCD_Init(void);

void nt_LCD_PrintLen(const char *str, int len);
void nt_LCD_Print(const char *str);
void nt_LCD_PrintChar(const char c);

#endif /* __NT_LCD_1602_H */
