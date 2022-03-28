#include "aw9523b.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "sdk_macros.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID 1

#define AW9523_ADDR 0b1011000

#define A0 0
#define A1 0

static const uint8_t aw9523_addr = AW9523_ADDR | (A0 << 1) | (A1 << 2);

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
            break;
    }
}

ret_code_t aw9523_write(uint8_t const* p_data, uint8_t length)
{
    ret_code_t err_code;

    m_xfer_done = false;
    err_code    = nrf_drv_twi_tx(&m_twi, aw9523_addr, p_data, length, false);
    VERIFY_SUCCESS(err_code);
    while (m_xfer_done == false)
        ;

    return NRF_SUCCESS;
}

ret_code_t aw9523_write_byte(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    ret_code_t err_code       = aw9523_write(buf, sizeof(buf));
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

ret_code_t aw9523_read_byte(uint8_t const reg, uint8_t* result)
{
    ret_code_t err_code;
    err_code = aw9523_write(&reg, sizeof(reg));
    VERIFY_SUCCESS(err_code);

    m_xfer_done = false;
    err_code    = nrf_drv_twi_rx(&m_twi, aw9523_addr, result, sizeof(*result));
    VERIFY_SUCCESS(err_code);
    while (m_xfer_done == false)
        ;

    return NRF_SUCCESS;
}

ret_code_t aw9523_init()
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
        .scl = 27, .sda = 26, .frequency = NRF_DRV_TWI_FREQ_400K, .interrupt_priority = APP_IRQ_PRIORITY_HIGH, .clear_bus_init = false};

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
    VERIFY_SUCCESS(err_code);

    nrf_drv_twi_enable(&m_twi);

    return NRF_SUCCESS;
}

ret_code_t aw9523_reset()
{
    uint8_t reg[2] = {AWREG_RESET, 0x00};
    ret_code_t err_code       = aw9523_write(reg, sizeof(reg));
    VERIFY_SUCCESS(err_code);
    return NRF_SUCCESS;
}
