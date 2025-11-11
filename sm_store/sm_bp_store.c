/*
 * sm_bp_store.c
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#include "sm_bp_store.h"
#include "sm_crc.h"
#include <stdint.h>

const uint32_t MAGIC_KEY_VALUE   = 0x5A5A5A5A;
#define FLASH_START_ADDR                (0x40100000U)
#define DATA_SIZE                       56
#define DATA_BUFFER_SIZE                64  // 58 + 8
#define FLASH_BLOCK_SIZE                (64U)
#define MAGIC_NUMBER_POS                0       // 4byte
#define CRC_BUFFER_POS                  4



typedef struct{
    sm_hal_flash_t* m_flash;
    uint32_t m_start_address;
}sm_bp_storage_impl_t;

#define _impl(x) (sm_bp_storage_impl_t*)(x)

int32_t sm_bp_storage_store(sm_bp_storage_t* _this, uint32_t _addr, uint32_t buf_j){
    sm_bp_storage_impl_t* this = _impl(_this);
       if(!this){
           return -1;
       }

       uint32_t load_buff[DATA_BUFFER_SIZE] = {0,};
       sm_hal_flash_read(this->m_flash, this->m_start_address, load_buff, DATA_BUFFER_SIZE);
       if(memcmp(load_buff + 2, buf_j + 2, DATA_SIZE) == 0) return 1;
       else{
           sm_hal_flash_write_block(this->m_flash, _addr, buf_j, DATA_BUFFER_SIZE);
       }







}



