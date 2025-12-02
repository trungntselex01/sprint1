/*
 * sm_hal_io.c
 *
 *  Created on: Nov 25, 2025
 *      Author: MYPC
 */

#include "stdlib.h"
#include "hal_data.h"
#include "sm_hal_io.h"

typedef struct {
    uint16_t m_pin;
    uint16_t m_port;
    sm_hal_io_mode_t m_mode;
}ra_io_t;

#define impl(x)   ((ra_io_t*)(x))

uint8_t GPIO_OPEN_FLAG = 0;

sm_hal_io_t* sm_hal_io_init(uint16_t _pin){
    ra_io_t *gpio = malloc(sizeof(ra_io_t));
    gpio->m_pin = _pin;
    return (sm_hal_io_t*)gpio;
}

void sm_hal_io_deinit(sm_hal_io_t *_this){
    if(!_this) return;
    free(_this);
}

int32_t sm_hal_io_open(sm_hal_io_t *_this, sm_hal_io_mode_t _mode){
    if(!_this) return -1;
    int32_t err;
    if(!GPIO_OPEN_FLAG){
        err = R_IOPORT_Open(g_ioport.p_ctrl, g_ioport.p_cfg);
        if(!err){
            return -1;
        }
        GPIO_OPEN_FLAG = 1;
    }
    uint32_t cfg;
    if(_mode == SM_HAL_IO_INPUT){
        cfg = IOPORT_CFG_PORT_DIRECTION_INPUT;
    }else {
        cfg = IOPORT_CFG_PORT_DIRECTION_OUTPUT;
    }
    err = R_IOPORT_PINCfg(g_ioport.p_ctrl, impl(_this)->m_pin, cfg);
    if(err) return -1;
    return 0;
}

int32_t sm_hal_io_close(sm_hal_io_t *_this){
    if(!_this) return -1;
    int32_t err = R_IOPORT_Close(g_ioport.p_ctrl);
    if(err) return -1;
    return 0;
}

int32_t sm_hal_io_set_value(sm_hal_io_t *_this, uint8_t _value){
    if(!_this) return -1;
    int32_t err = R_IOPORT_PinWrite(g_ioport.p_ctrl, impl(_this)->m_pin, _value);
    if(err) return -1;
    return 0;
}

uint8_t sm_hal_io_get_value(sm_hal_io_t *_this){
    if(!_this) return 0;
    uint8_t ret = 0;
    int32_t err = R_IOPORT_PinRead(g_ioport.p_ctrl, impl(_this)->m_pin, &ret);
    if(err) return 0;
    return ret;
}




