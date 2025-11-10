/*
 * sm_hal_flash.c

 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#include <stdbool.h>
#include "sm_hal_flash.h"
typedef struct{
    uint32_t m_block_size;
    uint32_t m_block_count;
    uint32_t m_start_addr;
    uint32_t m_end_addr;
}sm_hal_flash_impl_t;

#define _impl(x) (sm_hal_flash_impl_t*)(x)

bool sm_hal_flash_init(void){
    if (R_FLASH_HP_Open(&g_flash0_ctrl,&g_flash0_cfg) == FSP_SUCCESS) return true;
            else return false;
}

int32_t sm_hal_flash_get_block_size(sm_hal_flash_t *_this){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    return this->m_block_size;
}

int32_t sm_hal_flash_read(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this || _addr < this->m_start_addr ||(_addr + _size) > this->m_end_addr){
        return -1;
    }
//    sm_flash_protect(this);

    memcpy(_data, (uint8_t*)_addr, _size);

//    sm_flash_unprotect(this);

    return 0;
}

int32_t sm_hal_flash_erase_block(sm_hal_flash_t *_this, uint32_t _addr, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }
    uint8_t extra_block = (_size % this->m_block_size) ? 1: 0;
    uint32_t block_num = (uint32_t)(_size / this->m_block_size) + extra_block;

    sm_flash_protect(this);

    if(R_FLASH_HP_Erase(&g_flash0_ctrl, _addr, block_num) != FSP_SUCCESS){
        sm_flash_unprotect(this);
        return (-1);
    }

    flash_result_t blank_check_result = FLASH_RESULT_BLANK;
    uint32_t ret = R_FLASH_HP_BlankCheck(&g_flash0_ctrl, _addr, _size, &blank_check_result);

    sm_flash_unprotect(this);

    if(ret != FSP_SUCCESS || blank_check_result == FLASH_RESULT_NOT_BLANK){
        return -1;
    }

    return 0;
}

int32_t sm_hal_flash_write_block(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    if(sm_hal_flash_erase_block(this, _addr, _size) < 0){
        return -1;
    }

    sm_flash_protect(this);
    if(R_FLASH_HP_Write(&g_flash0_ctrl, (uint32_t)_data, _addr, _size) != FSP_SUCCESS){
        sm_flash_unprotect(this);
        return (-1);
    }

//    uint8_t read_buff[_size];
//    memcpy(read_buff,(uint8_t*)_addr, _size);
//
//    sm_flash_unprotect(this);
//
//    if(sm_flash_string_cmp((char*)read_buff, (char*)_data, _size) == 0 ){
//        LOG_ERR(TAG, "Verify write %d byte to addr 0x%x FAILED", _size, _addr);
//        return (-1);
//    }
//
//    return 0;
}





