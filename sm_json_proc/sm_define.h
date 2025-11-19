/*
 * sm_define.h
 *
 *  Created on: Nov 14, 2025
 *      Author: MYPC
 */

#ifndef SM_DEFINE_H_
#define SM_DEFINE_H_

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define SM_IOT_CMD_MAX                         64

#define SM_IOT_TYPE                           "type"
#define SM_IOT_TYPE_VAL                       "SET_CONFIG"
#define SM_IOT_DATA                           "data"
#define SM_IOT_BP_CONF_COV_MV                 "cov_mv"
#define SM_IOT_BP_CONF_CUV_MV                 "cuv_mv"
#define SM_IOT_BP_CONF_UTD_DEG                "utd_deg_c"
#define SM_IOT_BP_CONF_OTD_DEG                "otd_deg_c"
#define SM_IOT_BP_CONF_OCC_MA                 "occ_ma"
#define SM_IOT_BP_CONF_OCD_MA                 "ocd_ma"
#define SM_IOT_BP_CONF_SCD_MA                 "scd_ma"
#define SM_IOT_BP_CONF_DEBOUCE_MS             "debouce_ms"

#define ERR_STATUS                            "err_status"
#define SUCCESS                               "success"
#define ERR_TYPE                              "err_type"
#define ERR_VAL                               "err_value"



#define SM_IOT_BP_CONF_COV_MV_MAX             5000
#define SM_IOT_BP_CONF_COV_MV_MIN             4000
#define SM_IOT_BP_CONF_CUV_MV_MAX             5000
#define SM_IOT_BP_CONF_CUV_MV_MIN             2000
#define SM_IOT_BP_CONF_UTD_DEG_MAX            5
#define SM_IOT_BP_CONF_UTD_DEG_MIN            0
#define SM_IOT_BP_CONF_OTD_DEG_MAX            100
#define SM_IOT_BP_CONF_OTD_DEG_MIN            0
#define SM_IOT_BP_CONF_OCC_MA_MAX             20000
#define SM_IOT_BP_CONF_OCC_MA_MIN             15000
#define SM_IOT_BP_CONF_OCD_MA_MAX             60000
#define SM_IOT_BP_CONF_OCD_MA_MIN             50000
#define SM_IOT_BP_CONF_SCD_MA_MAX             90000
#define SM_IOT_BP_CONF_SCD_MA_MIN             70000
#define SM_IOT_BP_CONF_DEBOUCE_MS_MAX         4000
#define SM_IOT_BP_CONF_DEBOUCE_MS_MIN         2000
























#endif /* SM_DEFINE_H_ */

