/*
 * sm_hal_flash.h
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#ifndef SM_HAL_FLASH_H_
#define SM_HAL_FLASH_H_


#include <stdint.h>
#include "hal_data.h"
typedef struct sm_hal_flash_impl{
    uint32_t m_block_size;
    uint32_t m_block_number;
    uint32_t m_start_addr;
    uint32_t m_end_addr;
}sm_hal_flash_impl_t;

void flash_cb(flash_callback_args_t *p_args);

void sm_hal_flash_init(void);
sm_hal_flash_impl_t* sm_hal_flash_create(void);
int32_t sm_hal_flash_get_block_size(sm_hal_flash_impl_t *_this);
int32_t sm_hal_flash_write_block(sm_hal_flash_impl_t *_this, uint32_t _addr, void *_data, int32_t _size);
int32_t sm_hal_flash_read(sm_hal_flash_impl_t *_this, uint32_t _addr, void *_data, int32_t _size);
int32_t sm_hal_flash_erase_block(sm_hal_flash_impl_t *_this, uint32_t _addr, int32_t _size);

static inline void sm_flash_unprotect(){

    __enable_irq();
}


static inline void sm_flash_protect(){

    __disable_irq();
}

#endif /* SM_HAL_FLASH_H_ */
