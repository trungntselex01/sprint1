#include "hal_data.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

volatile uint64_t timer0_counter = 0;
typedef enum {
    SET_CONFIG = 0,
    SET_POWER  = 1,
    SET_LIMIT  = 2,
    TYPE_TOPIC_COUNT
} Topic_Type;
#define FLASH_START_ADDR (0x40100000U)
#define FLASH_BLOCK_SIZE      (64U)
#define FLASH_WORD_SIZE       (FLASH_BLOCK_SIZE / 4)
#define FLASH_TOTAL_BLOCKS    128U

#define MAGIC_NUMBER          0xA5A5A5A5UL
#define PAYLOAD_OFFSET   3U
#define PAYLOAD_WORDS    (FLASH_WORD_SIZE - PAYLOAD_OFFSET)

extern flash_ctrl_t g_flash0_ctrl;
extern const flash_cfg_t g_flash0_cfg;


bool sm_flash_init(void){
    if (R_FLASH_HP_Open(&g_flash0_ctrl,&g_flash0_cfg) == FSP_SUCCESS) return true;
    else return false;
}

bool sm_flash_erase(uint32_t addr, uint32_t num){
    if(R_FLASH_HP_Erase(&g_timer0_ctrl, addr, num) == FSP_SUCCESS) return true;
    else return false;
}

bool sm_flash_write(uint32_t addr, void *data, uint32_t size){
    if(R_FLASH_HP_Write(&g_flash0_ctrl,(uint32_t)data, addr, size) == FSP_SUCCESS) return true;
    else return false;
}

bool sm_flash_read(uint32_t addr, void *buf, uint32_t size){
    memcpy(buf, (uint32_t)addr, size);
    return true;
}

uint16_t crc16_compute(uint8_t *data, uint32_t len){
    uint16_t crc = 0xFFFF;
    for(uint32_t i = 0; i < len; i++){
        crc ^= (uint16_t)data[i] << 8;
                for (uint8_t j = 0; j < 8; j++)
                {
                    crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
         }
    }
    return crc;
}

int32_t sm_storage_find_empty_block(){
    uint32_t buf[FLASH_WORD_SIZE];
    for(uint32_t i = 0; i < FLASH_TOTAL_BLOCKS; i++){
        uint32_t addr = FLASH_START_ADDR + FLASH_BLOCK_SIZE * i;
        sm_flash_read(addr, buf, FLASH_BLOCK_SIZE);

        bool is_erased = true;
        for(uint32_t j = 0; j < FLASH_WORD_SIZE; j++){
            if(buf[j] != 0xFFFFFFFF){
                is_erased = false;
                break;
            }
        }
        if(is_erased) return i;
    }
    return -1;
}

int32_t sm_storage_find_lastest_block(){    // tìm block phù hợp
    uint32_t buf[FLASH_WORD_SIZE];
    int32_t last_valid = -1;
    for(uint32_t i = 0; i < FLASH_TOTAL_BLOCKS; i++){
        uint32_t addr = FLASH_START_ADDR + FLASH_BLOCK_SIZE * i;
        sm_flash_read(addr, buf, FLASH_BLOCK_SIZE);
        if(buf[0] == MAGIC_NUMBER){
            if(buf[1] == Topic_Type){
                uint16_t stored_crc = (uint16_t) buf[2];
                uint16_t calc_crc = crc16_compute((uint8_t *)&buf[PAYLOAD_OFFSET], PAYLOAD_WORDS * 4)
                if(stored_crc == calc_crc) last_valid = i;
            }
        }
    }
    return last_valid;
}


bool storage_load(uint32_t *buf, const uint32_t *default_buf){
    if(!buf || !default_buf) return false;
    int32_t lastest_idx = sm_storage_find_lastest_block();
    if(lastest_idx < 0){
        memcpy(buf, default_buf, FLASH_BLOCK_SIZE);
        return false;
    }
    uint32_t addr = FLASH_START_ADDR + lastest_idx + FLASH_BLOCK_SIZE;
    sm_flash_read(addr, buf, FLASH_BLOCK_SIZE);
    return true;
}

bool sm_storage_compare(uint32_t *buf_cur, uint32_t *buf_j){
    if(!buf_cur || !buf_j) return false;
    for(uint32_t i = PAYLOAD_OFFSET; i < FLASH_WORD_SIZE; i++){
        if(buf_cur[i] != buf_j[i]) return true;
    }
    return false;
}

bool sm_storage_store(uint32_t *buf, uint32_t *buf_j, const uint32_t *default_buf){
   if(storage_load(buf, default_buf)){
       if(sm_storage_compare(buf, buf_j)){
           buf_j[0] = MAGIC_NUMBER;
           buf_j[1] = TOPIC_TYPE;
           buf_j[2] = crc16_compute((uint8_t *)&buf_j[PAYLOAD_OFFSET], PAYLOAD_WORDS * 4);
           uint32_t last_valid = sm_storage_find_lastest_block();
           if(last_valid){
               sm_flash_write(last_valid * FLASH_BLOCK_SIZE + FLASH_START_ADDR, buf_j, sizeof(buf_j));
           }
           else{
               uint32_t empty_idx = sm_storage_find_empty_block();
               if(empty_idx) sm_flash_write(empty_idx * FLASH_BLOCK_SIZE + FLASH_START_ADDR, buf_j, sizeof(buf_j));
           }
       }
   }
   return false;
}

bool sm_storage_init(void){
    return sm_flash_init();
}


void agt0_callback(timer_callback_args_t *p_args){

    (void) p_args;
    timer0_counter ++;
}


void hal_entry(void)
{
    __disable_irq ();

    fsp_err_t err = R_AGT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    if(err != FSP_SUCCESS) {};
    err = R_AGT_Start(&g_timer0_ctrl);
    __enable_irq ();
    while (1)
    {
    }

    //          -- test thử nghiệm  --
//    fsp_err_t err;
//    uint8_t write_buf[BLOCK_SIZE];
//    uint8_t read_buf[BLOCK_SIZE];
//    memset(write_buf, 0xFF, sizeof(write_buf));
//    write_buf[0] = 1;
//    write_buf[1] = 2;
//
//    err = R_FLASH_HP_Open(&g_flash0_ctrl, &g_flash0_cfg);
//    if(err != FSP_SUCCESS){
//        _BKPT();
//    }
//    err = R_FLASH_HP_Open(&g_flash0_ctrl, &g_flash0_cfg);
//    if(err != FSP_SUCCESS){
//            _BKPT();
//        }
//    err = R_FLASH_HP_Write(&g_flash0_ctrl, (uint32_t)write_buf, DATA_FLASH_ADDR, BLOCK_SIZE);
//    if (err != FSP_SUCCESS)
//        {
//            _BKPT();
//        }
//    memcpy(read_buf, (uint8_t)DATA_FLASH_ADDR, BLOCK_SIZE);
//    if(read_buf[0] == 1 && read_buf[1] == 2){
//        _BKPT();
//    }
    //          -- test thử nghiệm  --



    // code sprint1
}





