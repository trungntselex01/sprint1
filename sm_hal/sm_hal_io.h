/*
 * sm_hal_io.h
 *
 *  Created on: Nov 25, 2025
 *      Author: MYPC
 */

#ifndef SM_HAL_IO_H_
#define SM_HAL_IO_H_

#include <stdint.h>
typedef enum {
    SM_HAL_IO_OFF = 0,
    SM_HAL_IO_ON
}sm_hal_io_value_t;

typedef enum{
    SM_HAL_IO_INPUT = 0,
    SM_HAL_IO_OUTPUT
}sm_hal_io_mode_t;

typedef void sm_hal_io_t;

sm_hal_io_t* sm_hal_io_init(uint16_t pin);
void sm_hal_io_deinit (sm_hal_io_t *_this);
int32_t sm_hal_io_set_value(sm_hal_io_t *_this, uint8_t value);
uint8_t sm_hal_io_get_value(sm_hal_io_t *_this);
int32_t sm_hal_io_open(sm_hal_io_t *_this, sm_hal_io_mode_t _mode);
int32_t sm_hal_io_close(sm_hal_io_t *_this);


#endif /* SM_HAL_IO_H_ */
