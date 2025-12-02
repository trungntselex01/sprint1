/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = agt_int_isr, /* AGT0 INT (AGT interrupt) */
            [1] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [2] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
            [3] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [4] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [5] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [6] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_AGT0_INT,GROUP0), /* AGT0 INT (AGT interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP1), /* FCU FRDYI (Flash ready interrupt) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,GROUP2), /* FCU FIFERR (Flash access error interrupt) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP3), /* SCI0 RXI (Receive data full) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP4), /* SCI0 TXI (Transmit data empty) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP5), /* SCI0 TEI (Transmit end) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP6), /* SCI0 ERI (Receive error) */
        };
        #endif
        #endif
