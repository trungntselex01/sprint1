/*
 * sm_hal_ble.c
 *
 *  Created on: Nov 25, 2025
 *      Author: MYPC
 */

#include "sm_hal_ble.h"


#define MDBT42_BUFFER_SIZE                      512

#define BT_RETRIES_INIT                         10
#define MODEM_BAUD_RATE_DF                      9600
#define MODEM_READ_BYTES_DELAY_MS(x)            (x)

#define BLE_RES_SUCCESS "success"
#define BLE_RES_FAIL    "fail"


