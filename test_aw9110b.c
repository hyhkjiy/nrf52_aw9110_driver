#include "extended/aw9110b.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "sdk_macros.h"

typedef enum {
    LED_LEFT_RED      = 0b001,
    LED_LEFT_GREEN    = 0b010,
    LED_LEFT_BLUE     = 0b100,
    LED_LEFT_YELLO    = 0b011,
    LED_LEFT_CYAN     = 0b110,
    LED_LEFT_MAGENTA  = 0b101,
    LED_LEFT_WHITE    = 0b111,
    LED_RIGHT_G       = 0b001000,
    LED_RIGHT_B       = 0b010000,
    LED_RIGHT_R       = 0b100000,
    LED_RIGHT_YELLO   = 0b101000,
    LED_RIGHT_CYAN    = 0b011000,
    LED_RIGHT_MAGENTA = 0b110000,
    LED_RIGHT_WHITE   = 0b111000,
} led_pin_t;

typedef enum {
    LED_FREQ_NONE = 0x07,
    LED_FREQ_FAST = 0x09,
    LED_FREQ_SLOW = 0x1B,
} led_frequency_t;

typedef enum {
    LED_FADE_NONE  = 0x00,
    LED_FADE_SHORT = 0x09,
    LED_FADE_LONG  = 0x12,
} led_fade_t;

/**
 * @brief 通过设置LED引脚、频率和淡入淡出来控制LED不同颜色状态的常亮、呼吸、闪烁等操作。
 *
 * @param pin led_pin_t
 * @param freq led_frequency_t
 * @param fade led_fade_t
 * @return ret_code_t
 */
static ret_code_t led_set(uint8_t pin, led_frequency_t freq, led_fade_t fade)
{
    VERIFY_SUCCESS(aw9110_reset(AW9110_LED_ADDR));
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P0, 0xFF ^ (pin >> 4)));      // OUT4~9配置为呼吸灯模式
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P1, 0xFF ^ (pin & 0b1111)));  // OUT0~3配置为呼吸灯模式
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_CONFIG_P0, 0x03));                      // OUT4-OUT5自主呼吸BLINK模式使能
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_CONFIG_P1, 0x0f));                      // OUT0-OUT3自主呼吸BLINK模式使能
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_FADE_TIME, fade));                      // 淡进淡出时间设置
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_FULL_TIME, freq));                      // 全亮全暗时间设置
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_EN_BREATH, 0x3f));                      // 自主呼吸使能
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_CTRL, 0x83));

    return NRF_SUCCESS;
}

void test_fade()
{
    aw9110_reset(AW9110_CTRL_ADDR);
    aw9110_write_byte(AW9110_CTRL_ADDR, AW91REG_WORK_MODE_P0, 0x00);    // OUT4~9配置为LED模式
    aw9110_write_byte(AW9110_CTRL_ADDR, AW91REG_WORK_MODE_P1, 0x00);    // OUT0~3配置为LED模式
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_EN_BREATH, 0x3f);        // 自主呼吸使能
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_CONFIG_P0, 0x00);        // OUT4-OUT5 SMART-FADE模式使能
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_CONFIG_P1, 0x00);        // OUT0-OUT3 SMART-FADE模式使能
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_FADE_TIME, 0b00100100);  // 淡进淡出时间设置

    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P0, 0x00);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0x00);

    nrf_delay_ms(5000);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P0, 0xFF);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0xFF);

    nrf_delay_ms(5000);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P0, 0x00);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0x00);

    nrf_delay_ms(5000);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P0, 0xFF);
    aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0xFF);
}

static ret_code_t led_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    VERIFY_SUCCESS(aw9110_reset(AW9110_LED_ADDR));
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P0, 0x00));  // OUT4~9配置为LED模式
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P1, 0x00));  // OUT0~3配置为LED模式

    uint8_t mode_p0, mode_p1;
    VERIFY_SUCCESS(aw9110_read_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P0, &mode_p0));
    VERIFY_SUCCESS(aw9110_read_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P1, &mode_p1));

    NRF_LOG_INFO("P0 MODE CONFIG: %u, P1 MODE CONFIG: %u", mode_p0, mode_p1);

    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_DIM00, r));  // OUT0~3配置为LED模式
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_DIM01, g));  // OUT0~3配置为LED模式
    VERIFY_SUCCESS(aw9110_write_byte(AW9110_LED_ADDR, AW91REG_DIM02, b));  // OUT0~3配置为LED模式

    return NRF_SUCCESS;
}

static void test_rgb()
{
    // aw9110_reset(AW9110_LED_ADDR);
    // aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P0, 0x00);  // OUT4~9配置为LED模式
    // aw9110_write_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P1, 0x00);  // OUT0~3配置为LED模式

    // uint8_t mode_p0, mode_p1;
    // aw9110_read_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P0, &mode_p0);
    // aw9110_read_byte(AW9110_LED_ADDR, AW91REG_WORK_MODE_P1, &mode_p1);

    // NRF_LOG_INFO("P0 MODE CONFIG: %u, P1 MODE CONFIG: %u", mode_p0, mode_p1);

    // uint8_t i = 0;
    // while (true) {
    //     aw9110_write_byte(AW9110_LED_ADDR, AW91REG_DIM00, i++);  // OUT0~3配置为LED模式
    //     nrf_delay_ms(20);
    // }
    while (true) {
        led_rgb(255, 0, 0);
        nrf_delay_ms(2000);

        led_rgb(0, 255, 0);
        nrf_delay_ms(2000);

        led_rgb(0, 0, 255);
        nrf_delay_ms(2000);

        led_rgb(255, 255, 0);
        nrf_delay_ms(2000);

        led_rgb(255, 0, 255);
        nrf_delay_ms(2000);

        led_rgb(0, 255, 255);
        nrf_delay_ms(2000);

        led_rgb(255, 255, 255);
        nrf_delay_ms(2000);
    }
}

static void test_gpio()
{
    aw9110_reset(AW9110_LED_ADDR);
    
    while (true) {
        aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0xFF);
        nrf_delay_ms(1000);
        aw9110_write_byte(AW9110_LED_ADDR, AW91REG_OUTPUT_P1, 0x00);
        nrf_delay_ms(1000);
    }
}

void test_aw9110b()
{
    // test_rgb();

    test_gpio();

    // aw9110_reset(AW9110_CTRL_ADDR);
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x12, 0x00);  // OUT4~9配置为呼吸灯模式
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x13, 0x00);  // OUT0~3配置为呼吸灯模式
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x04, 0x03);  // OUT4-OUT5自主呼吸BLINK模式使能
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x05, 0x0f);  // OUT0-OUT3自主呼吸BLINK模式使能
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x15, 0x12);  // 淡进淡出时间设置
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x16, 0x09);  // 全亮全暗时间设置
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x14, 0x3f);  // 自主呼吸使能
    // aw9110_write_byte(AW9110_CTRL_ADDR, 0x11, 0x82);

    // uint8_t led_pin = LED_RIGHT_CYAN | LED_LEFT_MAGENTA;

    // // // 慢闪烁
    // // led_set(LED_RIGHT_CYAN, LED_FREQ_FAST, LED_FADE_LONG);
    // // nrf_delay_ms(3000);

    // // // 慢闪烁
    // // led_set(LED_LEFT_MAGENTA, LED_FREQ_SLOW, LED_FADE_SHORT);
    // // nrf_delay_ms(3000);

    // while (true) {
    //     // 常亮
    //     led_set(led_pin, LED_FREQ_NONE, LED_FADE_NONE);
    //     nrf_delay_ms(3000);

    //     // 慢闪烁
    //     led_set(led_pin, LED_FREQ_SLOW, LED_FADE_NONE);
    //     nrf_delay_ms(3000);

    //     // 快闪烁
    //     led_set(led_pin, LED_FREQ_FAST, LED_FADE_NONE);
    //     nrf_delay_ms(3000);

    //     // 慢呼吸
    //     led_set(led_pin, LED_FREQ_SLOW, LED_FADE_LONG);
    //     nrf_delay_ms(3000);

    //     // 快呼吸
    //     led_set(led_pin, LED_FREQ_FAST, LED_FADE_SHORT);
    //     nrf_delay_ms(3000);
    // }

    NRF_LOG_INFO("config success!");
}