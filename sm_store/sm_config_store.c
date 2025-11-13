/*
 * sm_bp_store.c
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#include "sm_config_store.h"
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

#define MAGIC_NUMBER          0xA5A5A5A5UL



#define _impl(x)        ((sm_bp_storage_impl_t*)(x))


sm_config_storage_impl_t* sm_config_storage_create(sm_hal_flash_impl_t *m_flash
                                            , uint32_t addr, uint32_t size){
    if(!m_flash) return NULL;
    sm_config_storage_impl_t *g_config_storage = (sm_config_storage_impl_t*)
            malloc (sizeof(sm_config_storage_impl_t));
    g_config_storage->m_flash = m_flash;
    g_config_storage->m_start_address = addr;
    g_config_storage->m_size = size;

    return g_config_storage;
}

int32_t sm_hal_flash_store(sm_config_storage_impl_t *_this, void *_data){

    if (!_this)
    {
        return -1;
    }

    uint32_t start_offset = _this->m_start_address - _this->m_flash->m_start_addr;
    uint32_t end_offset = start_offset + _this->m_size + 4 + 2;

    uint32_t start_block = start_offset / _this->m_flash->m_block_size;
    uint32_t end_block = (end_offset + _this->m_flash->m_block_size - 1) / _this->m_flash->m_block_size; // làm tròn lên

    uint32_t block_count = end_block - start_block;

    // Tổng vùng cần thao tác (địa chỉ từ đầu block đầu -> cuối block cuối)
    uint32_t erase_start_addr = _this->m_flash->m_start_addr + start_block * _this->m_flash->m_block_size;
    uint32_t total_size = block_count * _this->m_flash->m_block_size;

    // Cấp buffer tạm để lưu toàn bộ vùng

    uint8_t buffer[total_size];
    memset(buffer,0,total_size);    // Đọc dữ liệu cũ trước khi xóa
    if (sm_hal_flash_read (_this->m_flash, erase_start_addr, buffer, total_size) != 0){
        return -3;
    }
    uint32_t write_index = (_this->m_start_address - erase_start_addr) / 1;
    if(memcmp(buffer + write_index + 4 + 2, _data, _this->m_size) == 0) return 1;

    uint16_t calc_crc = sm_CRC_CalculateCRC16((uint8_t*)_data, _this->m_size);

    uint32_t magic = MAGIC_NUMBER;
    memcpy(buffer + write_index, &magic, sizeof(magic));
    memcpy(buffer + write_index + 4, &calc_crc, 2);
    memcpy(buffer + write_index + 4 + 2, _data, _this->m_size);

    //  Ghi lại toàn bộ vùng (đã có dữ liệu cũ + mới)
    if (sm_hal_flash_write_block (_this->m_flash, erase_start_addr, buffer, total_size) != 0)
    {
        return -5;
    }
    return 0; //
}

int32_t sm_hal_flash_load(sm_config_storage_impl_t *_this, void *_data){
    uint32_t total_size = _this->m_size + 4 +2 ;
    uint8_t buff[total_size];

    sm_hal_flash_read(_this->m_flash,_this->m_start_address, buff, total_size);
    uint32_t magic_val;
    memcpy(&magic_val, buff, 4);
    if(magic_val != MAGIC_NUMBER) return -2;
    uint16_t crc;
    memcpy(&crc, buff + 4, 2);
    uint16_t calc_crc = sm_CRC_CalculateCRC16((uint8_t*)buff + 4 + 2, _this->m_size);
    if(crc != calc_crc) return -3;
    memcpy(_data, buff + 6, _this->m_size); return 0;
}




//
//int32_t sm_config_storage_store(sm_config_storage_impl_t* _this, void *_data){
//    if (!_this)
//        {
//            return -1;
//        }
//    uint8_t buff[(_this)->m_size];
//    memset(buff, 0, sizeof(buff));
//    if (sm_hal_flash_read (_this->m_flash, _this->m_start_address, buff, _this->m_size) != 0){
//            return -3;
//        }
//    int32_t cmp = memcmp(&buff, _data, _this->m_size);
//    if(cmp == 0) return 0;
//    else {
//        if(sm_hal_flash_store(_this, _data) != 0){
//            sm_hal_flash_store(_this, buff);
//            return -1;
//        }
//    }
//    return 1;
//
//}

//int32_t sm_config_storage_store(sm_config_storage_impl_t* _this, void *_data){
//    if (!_this)
//        {
//            return -1;
//        }
//    uint32_t buff[(_this)->m_size/4];
//    memset(buff, 0, sizeof(buff));
//    if (sm_hal_flash_read (_this->m_flash, _this->m_start_address, buff, _this->m_size) != 0){
//            return -3;
//        }
//    int32_t cmp = memcmp(buff, _data, _this->m_size);
//    if(cmp == 0) return 0;
//    else {
//        if(sm_hal_flash_store(_this, _data) != 0) return -1;
//        uint32_t verify_buff[_this->m_size / 4];
//            if (sm_hal_flash_read(_this->m_flash, _this->m_start_address, verify_buff, _this->m_size) != 0)
//                return -4;
//
//            uint32_t magic_val = verify_buff[0];
//            uint32_t crc       = verify_buff[1];
//
//        if(magic_val != _this->m_magic_val
//                || crc != sm_CRC_CalculateCRC16((uint8_t*)buff + 8, _this->m_size - 8)){
//            sm_hal_flash_store_block(_this, buff);
//            return -1;
//        }
//    }
//    return 1;
//}



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




