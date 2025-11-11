#include "hal_data.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_bp_store.h"
#include "sm_hal_flash.h"
volatile uint64_t timer0_counter = 0;
#define BUF_SIZE_WORDS   18
#define FLASH_CONFIG_DATA   (0x08000000U)
#define FLASH_ENERGY_DATA   (0x08000000U) + 16 + 4   // ghi thong so nang luong vao day
#define FLASH_START_ADDR    (0x08000000)
#define FLASH_END_ADDR      0x08001FFF
#define FLASH_BLOCK_SIZE      (64U)
#define FLASH_WORD_SIZE       (FLASH_BLOCK_SIZE / 4)
#define FLASH_TOTAL_BLOCKS    128U

#define MAGIC_NUMBER          0xA5A5A5A5UL
#define PAYLOAD_OFFSET   3U
#define PAYLOAD_WORDS    (FLASH_WORD_SIZE - PAYLOAD_OFFSET)



void agt0_callback(timer_callback_args_t *p_args){

    (void) p_args;
    timer0_counter ++;
}

uint8_t read_buffer[4] = {0, 0, 0, 0};
uint8_t read_energy[3] = {0, 0, 0};

typedef struct sprint1_t {

    sm_hal_flash_impl_t* m_hal_flash ;
    sm_bp_storage_impl_t* m_bp_config;
    sm_bp_storage_impl_t* m_bp_energy;
}sprint1;

sprint1 sprint_app;
void hal_entry(void)
{
    __disable_irq ();

    fsp_err_t err = R_AGT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    if(err != FSP_SUCCESS) {};
    err = R_AGT_Start(&g_timer0_ctrl);
    sprint_app.m_hal_flash = sm_hal_flash_create();

    sprint_app.m_bp_config = sm_bp_storage_create (sprint_app.m_hal_flash,
                                                   FLASH_CONFIG_DATA, sizeof(read_buffer));
    sprint_app.m_bp_energy = sm_bp_storage_create (sprint_app.m_hal_flash,
                                                   FLASH_ENERGY_DATA, sizeof(read_energy));

     uint8_t data_buffer[4] = {1, 5, 3, 4};
     sm_hal_flash_store_block(sprint_app.m_bp_config, data_buffer);
     R_BSP_SoftwareDelay(1, 1000);
     sm_hal_flash_read(sprint_app.m_hal_flash, FLASH_CONFIG_DATA, read_buffer, sizeof(read_buffer));
     if(read_buffer[0] == data_buffer[0] ) {

//         __BKPT(0);
     }


     // triển khai ghi năng lượng vào đây
//     uint32_t write_energy[3] = {5,6,7};
////     sm_hal_flash_store_block((sm_hal_flash_impl_t *) &sm_bp_store, FLASH_ENERGY_DATA, write_energy, sizeof(write_energy));
////     sm_hal_flash_read((sm_hal_flash_impl_t *)&sm_hal_flash, FLASH_ENERGY_DATA, read_energy, sizeof(read_buffer));
//     if(read_energy[0] == write_energy[0]){
//        // __BKPT(1);
//     }


    __enable_irq ();

    while (1)
    {

    }

    // code sprint1
}

void flash_cb(flash_callback_args_t *p_args){

    (void) p_args;
}
