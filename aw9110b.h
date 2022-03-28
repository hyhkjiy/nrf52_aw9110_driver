#ifndef AW9110B_H
#define AW9110B_H
#include "sdk_errors.h"

#define AW91REG_INPUT_P0 0x00
#define AW91REG_INPUT_P1 0x01
#define AW91REG_OUTPUT_P0 0x02
#define AW91REG_OUTPUT_P1 0x03
#define AW91REG_CONFIG_P0 0x04
#define AW91REG_CONFIG_P1 0x05
#define AW91REG_INT_P0 0x06
#define AW91REG_INT_P1 0x07
#define AW91REG_ID 0x10
#define AW91REG_CTRL 0x11
#define AW91REG_WORK_MODE_P0 0x12
#define AW91REG_WORK_MODE_P1 0x13
#define AW91REG_EN_BREATH 0x14
#define AW91REG_FADE_TIME 0x15
#define AW91REG_FULL_TIME 0x16
#define AW91REG_DLY0_BREATH 0x17
#define AW91REG_DLY1_BREATH 0x18
#define AW91REG_DLY2_BREATH 0x19
#define AW91REG_DLY3_BREATH 0x1a
#define AW91REG_DLY4_BREATH 0x1b
#define AW91REG_DLY5_BREATH 0x1c
#define AW91REG_DIM00 0x20
#define AW91REG_DIM01 0x21
#define AW91REG_DIM02 0x22
#define AW91REG_DIM03 0x23
#define AW91REG_DIM04 0x24
#define AW91REG_DIM05 0x25
#define AW91REG_DIM06 0x26
#define AW91REG_DIM07 0x27
#define AW91REG_DIM08 0x28
#define AW91REG_DIM09 0x29
#define AW91REG_SWRST 0x7F

#define AW9110_DEF_ID 0x58

#define AW9110_CTRL_ADDR 0b1011000
#define AW9110_LED_ADDR 0b1011011

ret_code_t aw9110_write(uint8_t const addr, uint8_t const* p_data, uint8_t length);
ret_code_t aw9110_write_byte(uint8_t const addr, uint8_t reg, uint8_t data);
ret_code_t aw9110_read_byte(uint8_t const addr, uint8_t const reg, uint8_t* result);
ret_code_t aw9110_init();
ret_code_t aw9110_reset(uint8_t const addr);

#endif