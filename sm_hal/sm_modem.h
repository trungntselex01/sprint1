/*
 * sm_model.h
 *
 *  Created on: Nov 28, 2025
 *      Author: MYPC
 */

#ifndef SM_MODEM_H_
#define SM_MODEM_H_

#include <stdbool.h>
#include "sm_hal_uart.h"
#include "sm_hal_io.h"

typedef struct sm_modem sm_modem_t;

struct sm_modem{
    sm_hal_io_t *reset_pin;
    sm_hal_uart_t *driver;
    char *buff;
    int32_t buff_index;
    bool lock;
    uint32_t buff_size;
};

void sm_modem_init(sm_modem_t *modem, sm_hal_io_t *io, sm_hal_uart_t *driver, uint32_t _size);
void sm_modem_deinit(sm_modem_t *modem);
int32_t sm_modem_send_cmd(sm_modem_t *modem, char* cmd, char* res_succ, char* res_fail);
int32_t sm_modem_send_recv(sm_modem_t *modem, char* cmd, char* res_succ, char* res_fail, char* res_buff, uint32_t res_buff_size);
void sm_modem_hw_reset(sm_modem_t *modem, uint32_t duration_ms, int logic);
int32_t sm_modem_write(sm_modem_t *modem, char* data, int32_t len);
int32_t sm_modem_read(sm_modem_t *modem, char* data, uint32_t len);
void modem_recv_data_callback(uint8_t _data, void *_arg);


#endif /* SM_MODEM_H_ */
