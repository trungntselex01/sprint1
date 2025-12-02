/*
 * sm_hal_uart.c
 *
 *  Created on: Nov 25, 2025
 *      Author: MYPC
 */
#include "sm_hal_uart.h"
#include <stdlib.h>
#include <stdbool.h>
#include "bsp_delay.h"


#define UART_RX_BUFFER 2048
#define SCI_UART_BAURATE_ERROR_PERCENT_5 (5000)
typedef struct {
    sm_hal_uart_rx_irq_fn_t m_rx_cb;
    void *m_rx_arg;
    sm_hal_uart_tx_irq_fn_t m_tx_cb;
    void *m_tx_arg;
}uart_irq_t;

typedef struct{
    uart_instance_t *m_chanel;
    uint32_t m_baud;
    uint8_t m_stop_bit;
    uint8_t m_data_bit;
    uint8_t m_tx_completed;
    uart_irq_t irq;
}ra_uart_t;

#define impl(x) ((ra_uart_t*)(x))

sm_hal_uart_t* sm_hal_uart_init(const void* _channel, uint32_t _baud, uint8_t _stop_bit, uint8_t _data_bit){
    if(!_channel) return NULL;
    ra_uart_t *uart = NULL;
    uart = maloc(sizeof(ra_uart_t));
    uart->m_chanel = (uart_instance_t*)_channel;
    uart->m_baud = _baud;
    uart->m_data_bit = _data_bit;
    uart->m_stop_bit = _stop_bit;
    uart->m_tx_completed = 1;
    uart->irq.m_rx_arg = NULL;
    uart->irq.m_rx_cb = NULL;
    uart->irq.m_tx_arg = NULL;
    uart->irq.m_tx_cb = NULL;
    return (sm_hal_uart_t*)uart;
}

void sm_hal_uart_deinit(sm_hal_uart_t* _this){
    if(!_this) return;
    free(_this);
}

int32_t sm_hal_uart_config(sm_hal_uart_t* _this, uint8_t _baud, uint8_t _stop_bit, uint8_t _data_bit){
    if(!_this) return -1;
    impl(_this)->m_baud = _baud;
    impl(_this)->m_data_bit = _data_bit;
    impl(_this)->m_stop_bit = _stop_bit;
    baud_setting_t baud_setting;
    uint32_t baud_rate = _baud;
    bool enable_bitrate_modulation = false;
    uint32_t error_rate_x_1000 = SCI_UART_BAURATE_ERROR_PERCENT_5;
    R_SCI_UART_BaudCaculate(baud_rate, enable_bitrate_modulation, error_rate_x_1000, &baud_setting);
    int32_t err = R_SCI_UART_Bauset(impl(_this)->m_chanel->p_ctrl, (void *)&baud_setting);
    return err?-1:0;
}

int32_t sm_hal_uart_write(sm_hal_uart_t* _this, uint8_t *_buff, uint32_t _len){
    if(!_this) return -1;
    int32_t err = R_SCI_UART_Write(impl(_this)->m_chanel->p_ctrl, _buff, _len);
    impl(_this)->m_tx_completed = 0;
    return err ? -1 : 0;
}

int32_t sm_hal_uart_read(sm_hal_uart_t* _this, uint8_t buff, uint32_t len){
    if(!_this) return -1;
    uint32_t length = 0;
    return length;
}

void sm_hal_uart_set_rx_cb(sm_hal_uart_t* _this, sm_hal_uart_rx_irq_fn_t _cb, void *_arg){
    if(!_this) return;
    impl(_this)->irq.m_rx_arg = _arg;
    impl(_this)->irq.m_rx_cb = _cb;
}

void sm_hal_uart_set_tx_cb(sm_hal_uart_t* _this, sm_hal_uart_rx_irq_fn_t _cb, void *_arg){
    if(!_this) return;
    impl(_this)->irq.m_tx_arg = _arg;
    impl(_this)->irq.m_tx_cb = _cb;
}

int32_t sm_hal_uart_open(sm_hal_uart_t *_this){
    if(!_this) return -1;
    int32_t err = R_SCI_UART_Open(impl(_this)->m_chanel->p_ctrl, impl(_this)->m_chanel->p_cfg);
    return err ? -1 : 0;
}

int32_t sm_hal_uart_close(sm_hal_uart_t *_this){
    if(!_this) return -1;
    int32_t err = R_SCI_UART_Close(impl(_this)->m_chanel->p_ctrl);
    return err ? -1 : 0;
}

int32_t sm_hal_uart_rx_irq(sm_hal_uart_t *_this, uint8_t _data){
    if(!_this) return -1;
    int8_t temp_data = _data;
    bool err;
    if(impl(_this)->irq.m_rx_cb){
        impl(_this)->irq.m_rx_cb(temp_data, impl(_this)->irq.m_rx_arg);
    }else{
        err = true;
    }
    return err ? -1 : 0;
}

int32_t sm_hal_uart_tx_irq(sm_hal_uart_t *_this, uint8_t _data){
    if(!_this) return -1;
    (void) _data;
    impl(_this)->m_tx_completed = 1;
    if(impl(_this)->irq.m_tx_cb){
            impl(_this)->irq.m_tx_cb(impl(_this)->irq.m_tx_arg);
            return 0;
    }else {
        return -1;
    }
}







