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






#define _impl(x) (sm_bp_storage_impl_t*)(x)



int32_t sm_hal_flash_store_block(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_bp_storage_impl_t* this = _impl(_this);
        sm_hal_flash_impl_t *that;
        if(!this || _addr < that->m_start_addr ||(_addr + _size) > that->m_end_addr){
               return -1;
           }

        uint32_t start_offset = this->m_start_address - that->m_start_addr;
        uint32_t end_offset   = start_offset + _size;

        uint32_t start_block = start_offset / that->m_block_size;
        uint32_t end_block   = (end_offset + that->m_block_size - 1) / that->m_block_size; // làm tròn lên

        uint32_t block_count  = end_block - start_block;

            // Tổng vùng cần thao tác (địa chỉ từ đầu block đầu -> cuối block cuối)
            uint32_t erase_start_addr = that->m_start_addr + start_block * that->m_block_size;
            uint32_t total_size = block_count * that->m_block_size;

            // Cấp buffer tạm để lưu toàn bộ vùng
            uint8_t *temp_buf = malloc(total_size);
            if (!temp_buf) return -2;

            // Đọc dữ liệu cũ trước khi xóa
            if (sm_hal_flash_read(_this, erase_start_addr, temp_buf, total_size) != 0) {
                free(temp_buf);
                return -3;
            }

            //  Ghi lại toàn bộ vùng (đã có dữ liệu cũ + mới)
            if (sm_hal_flash_write_block(_this, erase_start_addr, temp_buf, total_size) != 0) {
                free(temp_buf);
                return -5;
            }

            free(temp_buf);
            return 0; //
        }


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




