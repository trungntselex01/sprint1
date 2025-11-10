/*
 * sm_bp_store.c
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#include "sm_bp_store.h"
#include <stdint.h>

const uint32_t MAGIC_KEY_VALUE   = 0x5A5A5A5A;
#define FLASH_START_ADDR                (0x40100000U)
#define ODO_BUFFER_SIZE                 64
#define FLASH_BLOCK_SIZE                (64U)
#define MAGIC_NUMBER_POS                0       // 4byte
#define CRC_BUFFER_POS                  4



//typedef struct{
//    sm_hal_flash_t* m_flash;
//    uint32_t m_start_address;
//}sm_bp_storage_impl_t;
//
//#define _impl(x) (sm_bp_storage_impl_t*)(x)
//
//int32_t sm_bp_storage_store(sm_bp_storage_t* _this, uint32_t* _bp_store, uint32_t _addr ){
//    sm_bp_storage_impl_t* this = _impl(_this);
//       if(!this){
//           return -1;
//       }
//       int32_t sm_hal_flash_read(_this, _addr, void *_data, int32_t _size)
//}



