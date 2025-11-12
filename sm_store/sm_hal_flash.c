/*
 * sm_hal_flash.c

 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#include <stdbool.h>
#include "sm_hal_flash.h"
#include "string.h"
#include "hal_data.h"
#include "r_flash_api.h"


static fsp_err_t blankcheck_event_flag(void);

static volatile _Bool g_b_flash_event_not_blank = false;
static volatile _Bool g_b_flash_event_blank = false;
static volatile _Bool g_b_flash_event_erase_complete = false;
static volatile _Bool g_b_flash_event_write_complete = false;

void flash_cb(flash_callback_args_t *p_args){

    switch(p_args->event){

    case FLASH_EVENT_NOT_BLANK:
        g_b_flash_event_not_blank = true;
        break;

    case FLASH_EVENT_BLANK:
        g_b_flash_event_blank = true;
        break;

    case FLASH_EVENT_ERASE_COMPLETE:
        g_b_flash_event_erase_complete = true;
        break;

    case FLASH_EVENT_WRITE_COMPLETE:
        g_b_flash_event_write_complete = true;
        break;

    default:
        break;
    }
}


#define _impl(x) (sm_hal_flash_impl_t*)(x)

void sm_hal_flash_init(void){
    R_FLASH_HP_Open(&g_flash0_ctrl,&g_flash0_cfg);
}

static sm_hal_flash_impl_t g_hal_flash;
sm_hal_flash_impl_t* sm_hal_flash_create(void){
    fsp_err_t err;
    flash_info_t g_flash_infor;
    err = R_FLASH_HP_InfoGet(&g_flash0_ctrl,&g_flash_infor);

    if(err!=FSP_SUCCESS) return NULL;

    g_hal_flash.m_start_addr = g_flash_infor.data_flash.p_block_array->block_section_st_addr;
    g_hal_flash.m_end_addr = g_flash_infor.data_flash.p_block_array->block_section_end_addr;
    g_hal_flash.m_block_size = g_flash_infor.data_flash.p_block_array->block_size;
    g_hal_flash.m_block_number = g_flash_infor.code_flash.p_block_array->block_size_write;
    sm_hal_flash_init();
    return &g_hal_flash;
}
int32_t sm_hal_flash_get_block_size(sm_hal_flash_impl_t *_this){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    return this->m_block_size;
}

int32_t sm_hal_flash_read(sm_hal_flash_impl_t *_this, uint32_t _addr, void *_data, int32_t _size){
    if(!_this || _addr < _this->m_start_addr ||(_addr + _size) > _this->m_end_addr){
        return -1;
    }
//    sm_flash_protect(this);

    memcpy(_data, (uint32_t*)_addr, _size);

//    sm_flash_unprotect(this);

    return 0;
}

int32_t sm_hal_flash_erase_block(sm_hal_flash_impl_t *_this, uint32_t _addr, int32_t _size){
    if(!_this){
        return -1;
    }
    uint8_t extra_block = (_size % _this->m_block_size) ? 1: 0;
    uint32_t block_num = (uint32_t)(_size / _this->m_block_size) + extra_block;

    sm_flash_protect(_this);

    if(R_FLASH_HP_Erase(&g_flash0_ctrl, _addr, block_num) != FSP_SUCCESS){
        sm_flash_unprotect(_this);
        return (-1);
    }
    sm_flash_unprotect(_this);


    flash_result_t blank_check_result = FLASH_RESULT_BLANK;
    uint32_t ret = R_FLASH_HP_BlankCheck(&g_flash0_ctrl, _addr, _size, &blank_check_result);


    if(ret != FSP_SUCCESS || blank_check_result == FLASH_RESULT_NOT_BLANK){
        return -1;
    }

    return 0;
}

int32_t sm_hal_flash_write_block(sm_hal_flash_impl_t *_this, uint32_t _addr, void *_data, int32_t _size){
    if(!_this){
        return -1;
    }

    if(sm_hal_flash_erase_block(_this, _addr, _size) < 0){
        return -1;
    }

    sm_flash_protect(_this);
    if(R_FLASH_HP_Write(&g_flash0_ctrl, (uint32_t)_data, _addr, _size) != FSP_SUCCESS){
        sm_flash_unprotect(_this);
        return (-1);
    }


    uint8_t read_buff[_size];
    memcpy(read_buff,(uint32_t*)_addr, _size);

    sm_flash_unprotect(_this);

    if(memcmp((char*)read_buff, (char*)_data, _size) == 0 ){

        return 0;
    }

    return -1;
}

static fsp_err_t blankcheck_event_flag(void){

    fsp_err_t err = FSP_SUCCESS;
    /* Wait for callback function to set flag */
    while (!(g_b_flash_event_not_blank || g_b_flash_event_blank));

    if (g_b_flash_event_not_blank){
        /* Reset Flag */
        g_b_flash_event_not_blank = false;
        return (fsp_err_t)FLASH_EVENT_NOT_BLANK;
    }
    else{
        /* Reset Flag */
        g_b_flash_event_blank = false;
    }
    return err;
}


//int32_t sm_hal_flash_store_block(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
//    sm_hal_flash_impl_t* this = _impl(_this);
//        if(!this){
//            return -1;
//        }
//
//}





