/*
 * sm_config_init.h
 *
 *  Created on: Nov 17, 2025
 *      Author: MYPC
 */

#ifndef SM_CONFIG_INIT_H_
#define SM_CONFIG_INIT_H_

typedef struct sm_msg{
    void *m_payload;
} sm_msg_t;

typedef struct bp_config_data{
    int32_t cov_mv;
    int32_t cuv_mv;
    int32_t utd_deg_c;
    int32_t otd_deg_c;
    int32_t occ_ma;
    int32_t ocd_ma;
    int32_t scd_ma;
    int32_t debouce_ms;
}bp_config_data_t;

#endif /* SM_CONFIG_INIT_H_ */
