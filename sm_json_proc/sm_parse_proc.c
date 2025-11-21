/*
 * sm_parse_proc.c
 *
 *  Created on: Nov 14, 2025
 *      Author: MYPC
 */

#include <stdint.h>
#include "sm_parse_proc.h"
#include "sm_define.h"

int32_t sm_topic_config_handle(sm_msg_t* _msg, void* _buff);

int32_t check_val(uint32_t val, uint32_t min_val, uint32_t max_val){
    if(val > max_val || val < min_val) return -2;
    return 1;
}






int32_t sm_topic_config_handle(sm_msg_t *_msg, void *_buff)
{
    json_t mem[SM_IOT_CMD_MAX];
    const json_t *json = json_create ((char*) _msg->m_payload, mem, ARRAY_SIZE(mem));
    if (!json)
    {
        return -1;
    }
    const json_t *type = json_getProperty (json, SM_IOT_TYPE);
    if (!type || JSON_TEXT != json_getType (type))
    {
        return -1;
    }
    if (strcmp (json_getValue (type), SM_IOT_TYPE_VAL) != 0)
        return -1;

    const json_t *data = json_getProperty (json, SM_IOT_DATA);
    if (!data || JSON_OBJ != json_getType (data))
    {
        return -1;
    }
    bp_config_data_t *buff = (bp_config_data_t*) _buff;
    int32_t ret = 1;

    uint32_t cov_mv_val;
    const json_t *cov_mv = json_getProperty (data, SM_IOT_BP_CONF_COV_MV);
    cov_mv_val = json_getInteger(cov_mv);
        if ( check_val (cov_mv_val, SM_IOT_BP_CONF_COV_MV_MIN, SM_IOT_BP_CONF_COV_MV_MAX) > 0)
        {
            buff->cov_mv = cov_mv_val;
        }
        else ret = -2;

    uint32_t cuv_mv_val;
    const json_t *cuv_mv = json_getProperty (data, SM_IOT_BP_CONF_CUV_MV);
    cuv_mv_val = json_getInteger(cuv_mv);
            if ( check_val (cuv_mv_val, SM_IOT_BP_CONF_CUV_MV_MIN, SM_IOT_BP_CONF_CUV_MV_MAX) > 0)
            {
                buff->cuv_mv = cuv_mv_val;
            }
            else ret = -2;


    uint32_t utd_deg_c_val;
    const json_t *utd_deg_c = json_getProperty (data, SM_IOT_BP_CONF_UTD_DEG);
    utd_deg_c_val = json_getInteger(utd_deg_c);
                if ( check_val (utd_deg_c_val, SM_IOT_BP_CONF_UTD_DEG_MIN, SM_IOT_BP_CONF_UTD_DEG_MAX) > 0)
                {
                    buff->utd_deg_c = utd_deg_c_val;
                }
                else ret = -2;


    uint32_t otd_deg_c_val;
    const json_t *otd_deg_c = json_getProperty (data, SM_IOT_BP_CONF_OTD_DEG);
    otd_deg_c_val = json_getInteger(otd_deg_c);
                    if ( check_val (otd_deg_c_val, SM_IOT_BP_CONF_OTD_DEG_MIN, SM_IOT_BP_CONF_OTD_DEG_MAX) > 0)
                    {
                        buff->otd_deg_c = otd_deg_c_val;
                    }
                    else ret = -2;

    uint32_t occ_ma_val;
    const json_t *occ_ma = json_getProperty (data, SM_IOT_BP_CONF_OCC_MA);
    occ_ma_val = json_getInteger(occ_ma);
                        if ( check_val (occ_ma_val, SM_IOT_BP_CONF_OCC_MA_MIN, SM_IOT_BP_CONF_OCC_MA_MAX) > 0)
                        {
                            buff->occ_ma = occ_ma_val;
                        }
                        else ret = -2;

    uint32_t ocd_ma_val;
    const json_t *ocd_ma = json_getProperty (data, SM_IOT_BP_CONF_OCD_MA);
    ocd_ma_val = json_getInteger(ocd_ma);
                            if ( check_val (ocd_ma_val, SM_IOT_BP_CONF_OCD_MA_MIN, SM_IOT_BP_CONF_OCD_MA_MAX) > 0)
                            {
                                buff->ocd_ma = ocd_ma_val;
                            }
                            else ret = -2;

    uint32_t scd_ma_val;
    const json_t *scd_ma = json_getProperty (data, SM_IOT_BP_CONF_SCD_MA);
    scd_ma_val = json_getInteger(scd_ma);
                                if ( check_val (scd_ma_val, SM_IOT_BP_CONF_SCD_MA_MIN, SM_IOT_BP_CONF_SCD_MA_MAX) > 0)
                                {
                                    buff->scd_ma = scd_ma_val;
                                }
                                else ret = -2;

    uint32_t debouce_ms_val;
    const json_t *debouce_ms = json_getProperty (data, SM_IOT_BP_CONF_DEBOUCE_MS);
    debouce_ms_val = json_getInteger(debouce_ms);
                                    if ( check_val (debouce_ms_val, SM_IOT_BP_CONF_DEBOUCE_MS_MIN, SM_IOT_BP_CONF_DEBOUCE_MS_MAX) > 0)
                                    {
                                        buff->debouce_ms = debouce_ms_val;
                                    }
                                    else ret = -2;
    return ret;
}

void sm_topic_config_response(int32_t _err, char *_buf){
    char *p = _buf;
    p = json_objOpen (p, NULL);

    if (_err == 1)
    {
        p = json_str (p, ERR_STATUS, SUCCESS);
    }
    if (_err == -1)
    {
        p = json_str (p, ERR_STATUS, ERR_TYPE);
    }
    if (_err == -2)
    {
        p = json_str (p, ERR_STATUS, ERR_VAL);
    }
    p = json_objClose (p);
    p = json_end (p);
    *p = '\0';
}
