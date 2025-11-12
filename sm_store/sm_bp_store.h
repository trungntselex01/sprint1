/*
 * sm_bp_store.h
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#ifndef SM_BP_STORE_H_
#define SM_BP_STORE_H_

#include "sm_hal_flash.h"

typedef void sm_bp_storage_t;

typedef struct sm_bp_storage_impl{
    sm_hal_flash_impl_t* m_flash;
    uint32_t m_start_address;
    uint32_t m_size;
}sm_bp_storage_impl_t;

sm_bp_storage_impl_t* sm_bp_storage_create( sm_hal_flash_impl_t* m_flash,uint32_t addr,uint32_t size);


int32_t sm_bp_storage_store(sm_bp_storage_t* _this, uint32_t _bp);

int32_t sm_bp_storage_store_try(sm_bp_storage_t* _this, uint32_t _bp);

int32_t sm_hal_flash_store_block(sm_bp_storage_impl_t *_this, void *_data);



#endif /* SM_BP_STORE_H_ */
