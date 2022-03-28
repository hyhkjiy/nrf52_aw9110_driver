#include "aw9110b.h"
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "sdk_macros.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID 1

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

static void twi_handler(nrf_drv_twi_evt_t const* p_event, void* p_context)
{
    switch (p_event->type) {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX) {
                // data_handler(m_sample);
            }
            m_xfer_done = true;
            break;
        default:
            NRF_LOG_INFO("Twi handler event type: %u", p_event->type);
            break;
    }
}

ret_code_t aw9110_write(uint8_t const addr, uint8_t const* p_data, uint8_t length)
{
    ret_code_t err_code;

    m_xfer_done = false;
    err_code    = nrf_drv_twi_tx(&m_twi, addr, p_data, length, false);
    VERIFY_SUCCESS(err_code);
    while (m_xfer_done == false)
        ;

    return NRF_SUCCESS;
}

ret_code_t aw9110_write_byte(uint8_t const addr, uint8_t reg, uint8_t data)
{
    uint8_t    buf[2]   = {reg, data};
    ret_code_t err_code = aw9110_write(addr, buf, sizeof(buf));
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

ret_code_t aw9110_read_byte(uint8_t const addr, uint8_t const reg, uint8_t* result)
{
    ret_code_t err_code;
    err_code = aw9110_write(addr, &reg, sizeof(reg));
    VERIFY_SUCCESS(err_code);

    m_xfer_done = false;
    err_code    = nrf_drv_twi_rx(&m_twi, addr, result, sizeof(*result));
    VERIFY_SUCCESS(err_code);
    while (m_xfer_done == false)
        ;

    return NRF_SUCCESS;
}

ret_code_t aw9110_init()
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
        .scl = BOARD_I2C_SCL, .sda = BOARD_I2C_SDA, .frequency = NRF_DRV_TWI_FREQ_400K, .interrupt_priority = APP_IRQ_PRIORITY_HIGH, .clear_bus_init = false};

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
    VERIFY_SUCCESS(err_code);

    nrf_drv_twi_enable(&m_twi);

    VERIFY_SUCCESS(aw9110_reset(AW9110_CTRL_ADDR));
    VERIFY_SUCCESS(aw9110_reset(AW9110_LED_ADDR));

    return NRF_SUCCESS;
}

ret_code_t aw9110_reset(uint8_t const addr)
{
    uint8_t    reg[2]   = {AW91REG_SWRST, 0x00};
    ret_code_t err_code = aw9110_write(addr, reg, sizeof(reg));
    VERIFY_SUCCESS(err_code);
    return NRF_SUCCESS;
}
