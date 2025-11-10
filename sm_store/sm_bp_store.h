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

int32_t sm_ev_odo_storage_load(sm_bp_storage_t* _this, uint32_t* _bp_storage);

int32_t sm_ev_odo_storage_store(sm_bp_storage_t* _this, uint32_t _bp);

#endif /* SM_BP_STORE_H_ */
