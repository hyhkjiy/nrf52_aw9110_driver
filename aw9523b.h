#ifndef AW9523B_H
#define AW9523B_H
#include "sdk_errors.h"

#define AWREG_READ_P0_INPUT 0x00
#define AWREG_READ_P1_INPUT 0x01
#define AWREG_P0_OUTPUT 0x02
#define AWREG_P1_OUTPUT 0x03
#define AWREG_CONFIG_P0 0x04
#define AWREG_CONFIG_P1 0x05
#define AWREG_ID 0x10
#define AWREG_CTL 0x11
#define AWREG_MODE_P0 0x12
#define AWREG_MODE_P1 0x13
#define AWREG_RESET 0x7F

#define AW9523_DEF_ID 0x23

ret_code_t aw9523_write(uint8_t const* p_data, uint8_t length);
ret_code_t aw9523_write_byte(uint8_t reg, uint8_t data);
ret_code_t aw9523_read_byte(uint8_t const reg, uint8_t* result);
ret_code_t aw9523_init();
ret_code_t aw9523_reset();

#endif