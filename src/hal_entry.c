#include <sm_config_store.h>
#include <stdbool.h>
#include "hal_data.h"
#include "sm_crc.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_hal_flash.h"
#include "sm_parse_proc.h"
#include "sm_define.h"
#include "sm_config_init.h"
volatile uint64_t timer0_counter = 0;           // (0x08000000U)    (0x08001FFF)
#define BUF_SIZE_WORDS   18
#define FLASH_MODE_DATA     (0x0800003CU)
#define FLASH_CONFIG_DATA   (0x08000140U)
#define FLASH_ENERGY_DATA   (0x08000000U)   // ghi thong so nang luong vao day
#define FLASH_START_ADDR    (0x08000000U)
#define FLASH_END_ADDR      0x08001FFFU
#define FLASH_BLOCK_SIZE      (64U)
#define FLASH_WORD_SIZE       (FLASH_BLOCK_SIZE / 4)
#define FLASH_TOTAL_BLOCKS    128U


#define PAYLOAD_OFFSET   3U
#define PAYLOAD_WORDS    (FLASH_WORD_SIZE - PAYLOAD_OFFSET)



int32_t test_parse_config(bp_config_data_t* _cfg);


bp_config_data_t bp_config_data_default = {
    .cov_mv     = 4200,
    .cuv_mv     = 3000,
    .utd_deg_c  = 0,
    .otd_deg_c  = 65,
    .occ_ma     = 19000,
    .ocd_ma     = 55000,
    .scd_ma     = 80000,
    .debouce_ms = 3000,
};

typedef struct sprint1_t {
    bp_config_data_t _cfg_;
    sm_hal_flash_impl_t* m_hal_flash ;
    sm_config_storage_impl_t* m_bp_config;
    sm_config_storage_impl_t* m_bp_energy;
    sm_config_storage_impl_t* m_bp_mode;
}sprint1;

sprint1 sprint_app;
bool reset_sys_flag = false;

int32_t test_parse_config(bp_config_data_t *_cfg)
    {
        const char test_json[] = "{"
                "  \"type\": \"SET_CONFIG\","
                "  \"data\": {"
                "    \"cov_mv\": 4600,"
                "    \"cuv_mv\": 3000,"
                "    \"utd_deg_c\": 0,"
                "    \"otd_deg_c\": 65,"
                "    \"occ_ma\": 19000,"
                "    \"ocd_ma\": 55000,"
                "    \"scd_ma\": 80000,"
                "    \"debouce_ms\": 3000"
                "  }"
                "}";

//        const char test_json[] = "{"
//                "  \"type\": \"SET_CONFIG\","
//                "  \"data\": {"
//                "    \"cov_mv\": 4600,"
//                "    \"cuv_mv\": 3000"
//                "  ";

        sm_msg_t msg;
        msg.m_payload = (char*) test_json;

        bp_config_data_t cfg;

        int32_t ret = sm_topic_config_handle (&msg, &cfg);
        *_cfg = cfg;

        char resp[128];
        sm_topic_config_response (ret, resp);
        return ret;
    }

void hal_entry(void)
{
    __disable_irq ();

    fsp_err_t err = R_AGT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    if(err != FSP_SUCCESS) {};
    err = R_AGT_Start(&g_timer0_ctrl);
    sprint_app.m_hal_flash = sm_hal_flash_create();

    sprint_app.m_bp_config = sm_config_storage_create (sprint_app.m_hal_flash,
                                                   FLASH_CONFIG_DATA, sizeof(bp_config_data_t));


//    test_parse_config(_cfg_);
    int32_t status_load = sm_hal_flash_load(sprint_app.m_bp_config, &sprint_app._cfg_);
    if(status_load == -2){
        sm_hal_flash_store(sprint_app.m_bp_config, &bp_config_data_default);
        memcpy(&sprint_app._cfg_, &bp_config_data_default, sizeof(bp_config_data_default));
    }

    __enable_irq ();

    while (1)
    {

        if(reset_sys_flag == true){

            __NVIC_SystemReset();
        }
        R_BSP_SoftwareDelay(10, 1000);
    }

    // code sprint1
}

bool check_json_flag = false;
void agt0_callback(timer_callback_args_t *p_args){
    (void)p_args;
    bp_config_data_t _cfg;
    if(check_json_flag == true){
    int32_t _ret = test_parse_config( &_cfg);
    if(_ret == 1){
    if(sm_hal_flash_store(sprint_app.m_bp_config, &_cfg) == 0){

        reset_sys_flag = true;
    }
    check_json_flag = false;
    }
    }

    timer0_counter ++;
    if(timer0_counter == 10000){
        check_json_flag = true;
    }

}

