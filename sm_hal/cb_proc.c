/*
 * cb_proc.c
 *
 *  Created on: Dec 2, 2025
 *      Author: MYPC
 */

#include "hal_data.h"
#include "sm_modem.h"


void ble_cb(uart_callback_args_t *_arg, void *arg){
    switch(_arg->event){
        case UART_EVENT_RX_COMPLETE:
        case UART_EVENT_TX_COMPLETE:
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
            break;
        case UART_EVENT_RX_CHAR:
            modem_recv_data_callback(_arg->data, arg);
            break;
    }
}

