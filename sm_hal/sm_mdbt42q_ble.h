/*
 * sm_mdbt42q.ble.h
 *
 *  Created on: Nov 28, 2025
 *      Author: MYPC
 */

#ifndef SM_MDBT42Q_BLE_H_
#define SM_MDBT42Q_BLE_H_

#include "sm_modem.h"

typedef struct {
    sm_modem_t  m_base;
    uint8_t     m_indicator_logic_on_connecting;
    void*       m_indicator_pin;
}sm_mdbt42q_t;

sm_mdbt42q_t* sm_mdbt42q_create(sm_hal_uart_t* _driver, sm_hal_io_t* _rst, sm_hal_io_t* _ind, sm_hal_io_t* _wk);
int32_t sm_mdbt42q_init(sm_mdbt42q_t* _this);
void sm_mdbt42q_deinit(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_set_name(sm_mdbt42q_t* _this, char* _name);
int32_t sm_mdbt42q_start_advertising(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_stop_advertising(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_disconnect(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_set_default(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_soft_reset(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_hard_reset(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_read_data(sm_mdbt42q_t* _this, char* _data, uint32_t _len);
int32_t sm_mdbt42q_write_data(sm_mdbt42q_t* _this, char* _data, uint32_t _len);
int32_t sm_mdbt42q_get_indicator_logic(sm_mdbt42q_t* _this);
int32_t sm_mdbt42q_is_connected(sm_mdbt42q_t* _this);











#endif /* SM_MDBT42Q_BLE_H_ */
