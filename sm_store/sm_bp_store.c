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


#define MAX_STORAGE_SP                  10



#define _impl(x)        ((sm_bp_storage_impl_t*)(x))


sm_bp_storage_impl_t* sm_bp_storage_create(sm_hal_flash_impl_t *m_flash
                                            , uint32_t addr, uint32_t size){
    if(!m_flash) return NULL;
    sm_bp_storage_impl_t *g_bp_storage = (sm_bp_storage_impl_t*)
            malloc (sizeof(sm_bp_storage_impl_t));
    g_bp_storage->m_flash = m_flash;
    g_bp_storage->m_start_address = addr;
    g_bp_storage->m_size = size;

    return g_bp_storage;
}

int32_t sm_hal_flash_store_block(sm_bp_storage_impl_t *_this, void *_data){

    if (!_this)
    {
        return -1;
    }

    uint32_t start_offset = _impl(_this)->m_start_address - _impl(_this)->m_flash->m_start_addr;
    uint32_t end_offset = start_offset + _impl(_this)->m_size;

    uint32_t start_block = start_offset / _impl(_this)->m_flash->m_block_size;
    uint32_t end_block = (end_offset + _impl(_this)->m_flash->m_block_size - 1) / _impl(_this)->m_flash->m_block_size; // làm tròn lên

    uint32_t block_count = end_block - start_block;

    // Tổng vùng cần thao tác (địa chỉ từ đầu block đầu -> cuối block cuối)
    uint32_t erase_start_addr = _impl(_this)->m_start_address + start_block * _impl(_this)->m_flash->m_block_size;
    uint32_t total_size = block_count * _impl(_this)->m_flash->m_block_size;

    // Cấp buffer tạm để lưu toàn bộ vùng

    uint8_t buffer[total_size];
    memset(buffer,0,total_size);    // Đọc dữ liệu cũ trước khi xóa
    if (sm_hal_flash_read (_this->m_flash, erase_start_addr, buffer, total_size) != 0){
        return -3;
    }

    uint32_t write_index = (_impl(_this)->m_start_address - erase_start_addr) / 1;
    memcpy(&buffer[0],_data, _this->m_size);
    //  Ghi lại toàn bộ vùng (đã có dữ liệu cũ + mới)
    if (sm_hal_flash_write_block (_this->m_flash, erase_start_addr, buffer, total_size) != 0)
    {
        return -5;
    }
    return 0; //
}

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




