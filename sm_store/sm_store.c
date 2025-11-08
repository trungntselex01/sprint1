/*
 * sm_store.c
 *
 *  Created on: 7 Nov 2025
 *      Author: My PC
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include  "sm_store.h"

uint32_t test_counter = 0;
void sm_store_test(void){
test_counter ++;
}

fsp_err_t err;

static fsp_err_t blankcheck_event_flag(void);
static volatile _Bool g_b_flash_event_not_blank = false;
static volatile _Bool g_b_flash_event_blank = false;
static volatile _Bool g_b_flash_event_erase_complete = false;
static volatile _Bool g_b_flash_event_write_complete = false;

void flash_cb(flash_callback_args_t *p_args){

    switch(p_args->event){

    case FLASH_EVENT_NOT_BLANK:
        g_b_flash_event_not_blank = true;
        break;

    case FLASH_EVENT_BLANK:
        g_b_flash_event_blank = true;
        break;

    case FLASH_EVENT_ERASE_COMPLETE:
        g_b_flash_event_erase_complete = true;
        break;

    case FLASH_EVENT_WRITE_COMPLETE:
        g_b_flash_event_write_complete = true;
        break;

    default:
        break;
    }
}

bool sm_flash_init(void){
        if (R_FLASH_HP_Open(&g_flash0_ctrl,&g_flash0_cfg) == FSP_SUCCESS) return true;
        else return false;
}

//bool sm_flash_erase(uint32_t addr, uint32_t num){
//    if(R_FLASH_HP_Erase(&g_timer0_ctrl, addr, num) == FSP_SUCCESS) return true;
//    else return false;
//}

fsp_err_t sm_flash_erase(uint32_t addr, uint32_t num_blocks)
{
    fsp_err_t err = R_FLASH_HP_Erase(&g_flash0_ctrl, addr, num_blocks);
    if (err != FSP_SUCCESS)
    {
        return err;
    }

    if (g_flash0_cfg.data_flash_bgo == true)
    {
        while (!g_b_flash_event_erase_complete)
        {
            // đợi callback báo complete
        }
        g_b_flash_event_erase_complete = false;
    }

    return FSP_SUCCESS;
}

//bool sm_flash_write(uint32_t addr, void *data, uint32_t size){
//    if(R_FLASH_HP_Write(&g_flash0_ctrl,(uint32_t)data, addr, size) == FSP_SUCCESS) return true;
//    else return false;
//}

fsp_err_t sm_flash_write(uint32_t addr, void *data, uint32_t size)
{
    fsp_err_t err = R_FLASH_HP_Write(&g_flash0_ctrl, (uint32_t)data, addr, size);
    if (err != FSP_SUCCESS)
    {
        return err;
    }

    if (g_flash0_cfg.data_flash_bgo == true)
    {
        while (!g_b_flash_event_write_complete)
        {
        }
        g_b_flash_event_write_complete = false;
    }

    return FSP_SUCCESS;
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

int32_t sm_storage_find_empty_block(uint32_t data_size)
{
    uint32_t buf[FLASH_WORD_SIZE];
    uint32_t needed_blocks = (data_size + FLASH_BLOCK_SIZE - 1) / FLASH_BLOCK_SIZE; // làm tròn lên
    uint32_t consecutive_free = 0;
    int32_t first_free_block = -1;

    for (uint32_t i = 0; i < FLASH_TOTAL_BLOCKS; i++)
    {
        uint32_t addr = FLASH_START_ADDR + FLASH_BLOCK_SIZE * i;
        sm_flash_read(addr, buf, FLASH_BLOCK_SIZE);

        bool is_erased = true;
        for (uint32_t j = 0; j < FLASH_WORD_SIZE; j++)
        {
            if (buf[j] != 0xFFFFFFFF)
            {
                is_erased = false;
                break;
            }
        }

        if (is_erased)
        {
            // bắt đầu chuỗi trống mới
            if (consecutive_free == 0)
                first_free_block = i;

            consecutive_free++;

            if (consecutive_free >= needed_blocks)
                return first_free_block; // đủ chỗ, trả về vị trí bắt đầu
        }
        else
        {
            // reset chuỗi trống
            consecutive_free = 0;
            first_free_block = -1;
        }
    }

    return -1;
}

int32_t sm_storage_find_lastest_block(uint32_t data_size){    // tìm block phù hợp
    uint32_t buf[data_size/sizeof(uint32_t)];
    int32_t last_valid = -1;
    for(uint32_t i = 0; i < FLASH_TOTAL_BLOCKS; i++){
        uint32_t addr = FLASH_START_ADDR + FLASH_BLOCK_SIZE * i;
        sm_flash_read(addr, buf, sizeof(buf));
        if(buf[0] == MAGIC_NUMBER){
            if(buf[1] == 1){
                uint16_t stored_crc = (uint16_t) buf[2];
                uint16_t calc_crc = crc16_compute((uint8_t *)&buf[PAYLOAD_OFFSET], (data_size/sizeof(uint32_t) - PAYLOAD_OFFSET))
                if(stored_crc == calc_crc) last_valid = i;
            }
        }
    }
    return last_valid;
}


bool storage_load(uint32_t *buf, const uint32_t *default_buf, uint32_t data_size){
    if(!buf || !default_buf) return false;
    int32_t lastest_idx = sm_storage_find_lastest_block(data_size);
    if(lastest_idx < 0){
        memcpy(buf, default_buf, sizeof(default_buf));
        return true;
    }
    uint32_t addr = FLASH_START_ADDR + lastest_idx + FLASH_BLOCK_SIZE;
    sm_flash_read(addr, buf, data_size);
    return true;
}

bool sm_storage_compare(uint32_t *buf_cur, uint32_t *buf_j, uint32_t data_size){
    if(!buf_cur || !buf_j) return false;
    for(uint32_t i = PAYLOAD_OFFSET; i < data_size/sizeof(uint32_t); i++){
        if(buf_cur[i] != buf_j[i]) return true;
    }
    return false;
}

bool sm_storage_store(uint32_t *buf, uint32_t *buf_j, const uint32_t *default_buf, uint32_t data_size){
   if(storage_load(buf, default_buf, data_size)){
       if(sm_storage_compare(buf, buf_j)){
           buf_j[0] = MAGIC_NUMBER;
           buf_j[1] = 1;
           buf_j[2] = crc16_compute((uint8_t *)&buf_j[PAYLOAD_OFFSET], (data_size)/sizeof(uint32_t) - PAYLOAD_OFFSET);
           uint32_t last_valid = sm_storage_find_lastest_block(data_size);
           if(last_valid){
               sm_flash_init();
               if(sm_flash_erase(last_valid * FLASH_BLOCK_SIZE + FLASH_START_ADDR, (data_size + FLASH_BLOCK_SIZE - 1) / FLASH_BLOCK_SIZE) == FSP_SUCCESS)
               {
               if(sm_flash_write(last_valid * FLASH_BLOCK_SIZE + FLASH_START_ADDR, buf_j, sizeof(buf_j)) == FSP_SUCCESS) return true;
               }
           }
           else{
               uint32_t empty_idx = sm_storage_find_empty_block();
               if(empty_idx){
                   sm_flash_init();
                   if(sm_flash_write(empty_idx * FLASH_BLOCK_SIZE + FLASH_START_ADDR, buf_j, sizeof(buf_j)) == FSP_SUCCESS) return true;
               }
           }
       }
   }
   return false;
}

