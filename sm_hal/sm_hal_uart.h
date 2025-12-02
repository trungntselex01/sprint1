/*
 * sm_hal_uart.h
 *
 *  Created on: Nov 25, 2025
 *      Author: MYPC
 */

#ifndef SM_HAL_UART_H_
#define SM_HAL_UART_H_

#include <stdio.h>
typedef enum{
    SM_UART_PARITY_NONE = 0,
    SM_UART_PARITY_ODD,
    SM_UART_PARITY_EVEN
}SM_UART_PARITY;

typedef enum{
    SM_UART_DATA_BIT7 = 0,
    SM_UART_DATA_BIT8,
    SM_UART_DATA_BIT9
}SM_UART_DATA_BIT;

typedef void sm_hal_uart_t;

typedef void (*sm_hal_uart_rx_irq_fn_t)(uint8_t, void*);
typedef void (*sm_hal_uart_tx_irq_fn_t)(void*);

sm_hal_uart_t* sm_hal_uart_init(const void* _channel, uint32_t _baud, uint8_t _stop_bit, uint8_t _data_bit);
void sm_hal_uart_deinit(sm_hal_uart_t* _this);
int32_t sm_hal_uart_config(sm_hal_uart_t* _this, uint8_t _baud, uint8_t _stop_bit, uint8_t _data_bit);
int32_t sm_hal_uart_write(sm_hal_uart_t* _this, uint8_t *_buff, uint32_t _len);
int32_t sm_hal_uart_read(sm_hal_uart_t* _this, uint8_t buff, uint32_t len);
void sm_hal_uart_set_rx_cb(sm_hal_uart_t* _this, sm_hal_uart_rx_irq_fn_t _cb, void *_arg);
void sm_hal_uart_set_tx_cb(sm_hal_uart_t* _this, sm_hal_uart_tx_irq_fn_t _cb, void *_arg);
int32_t sm_hal_uart_open(sm_hal_uart_t *_this);
int32_t sm_hal_uart_close(sm_hal_uart_t *_this);
int32_t sm_hal_uart_rx_irq(sm_hal_uart_t *_this, uint8_t _data);
int32_t sm_hal_uart_tx_irq(sm_hal_uart_t *_this, uint8_t _data);
int32_t sm_hal_uart_set_baud_rate(sm_hal_uart_t *_this, uint32_t baud);






#endif /* SM_HAL_UART_H_ */
