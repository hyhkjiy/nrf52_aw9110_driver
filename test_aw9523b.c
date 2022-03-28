#include "extended/aw9523b.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "sdk_macros.h"

void test_aw9523b()
{
    ret_code_t err_code = aw9523_reset();
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("aw9523 reseted");

    uint8_t chip_id;
    err_code = aw9523_read_byte(0x10, &chip_id);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("aw9523 id: %d", chip_id);

    // 设置P0为Push-Pull模式
    err_code = aw9523_write_byte(AWREG_CTL, 0b00010000);
    APP_ERROR_CHECK(err_code);

    // 设置P0为GPIO模式
    err_code = aw9523_write_byte(AWREG_MODE_P0, 0xFF);
    APP_ERROR_CHECK(err_code);

    // 设置P0为输出
    err_code = aw9523_write_byte(AWREG_CONFIG_P0, 0x00);
    APP_ERROR_CHECK(err_code);

    // 设置P0_5、P0_6输出高电平
    err_code = aw9523_write_byte(AWREG_P0_OUTPUT, 0b01100000);
    APP_ERROR_CHECK(err_code);

    uint8_t p0;
    err_code = aw9523_read_byte(0x02, &p0);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("aw9523 gpio0 status: %d", p0);

    NRF_LOG_INFO("set gpio success");

    while (1) {
        err_code = aw9523_write_byte(AWREG_P0_OUTPUT, 0b01000000);
        APP_ERROR_CHECK(err_code);

        err_code = aw9523_write_byte(AWREG_P0_OUTPUT, 0b00100000);
        APP_ERROR_CHECK(err_code);
    }
}