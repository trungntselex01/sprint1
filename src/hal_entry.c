#include "hal_data.h"
#include "stdint.h"

volatile uint64_t timer0_counter = 0;

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

}

