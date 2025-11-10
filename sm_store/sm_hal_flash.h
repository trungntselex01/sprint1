/*
 * sm_hal_flash.h
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#ifndef SM_HAL_FLASH_H_
#define SM_HAL_FLASH_H_


#include <stdint.h>

typedef void sm_hal_flash_t;
sm_hal_flash_t* sm_hal_flash_init(void);
int32_t sm_hal_flash_get_block_size(sm_hal_flash_t *_this);
int32_t sm_hal_flash_write_block(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size);
int32_t sm_hal_flash_read(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size);
int32_t sm_hal_flash_erase_block(sm_hal_flash_t *_this, uint32_t _addr, int32_t _size);



#endif /* SM_HAL_FLASH_H_ */
