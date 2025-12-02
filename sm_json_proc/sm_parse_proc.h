/*
 * sm_parse_proc.h
 *
 *  Created on: Nov 14, 2025
 *      Author: MYPC
 */

#ifndef SM_PARSE_PROC_H_
#define SM_PARSE_PROC_H_

#include "sm_json_maker.h"
#include "sm_tiny_json.h"
#include "sm_config_init.h"



int32_t sm_topic_config_handle(sm_msg_t* _msg, void* _buff);

void sm_topic_config_response(int32_t _err, char *_buf);

int32_t check_val(uint32_t val, uint32_t min_val, uint32_t max_val);

int32_t test_parse_config_ble(bp_config_data_t *_cfg, char test_json);


#endif /* SM_PARSE_PROC_H_ */
