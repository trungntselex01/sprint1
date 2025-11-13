/*
 * sm_crc.h
 *
 *  Created on: Nov 10, 2025
 *      Author: MYPC
 */

#ifndef SM_CRC_H_
#define SM_CRC_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


#define RUNTTIME							0
#define TABLE								1
#define HARDWARE							2


/* ---------- Defines for 8-bit SAE J1850 CRC calculation (Not reflected) ------------------------------------------------------- */
#define CRC_8_RESULT_WIDTH                  8u
#define CRC_8_POLYNOMIAL                    0x1Du
#define CRC_8_INIT_VALUE                    0xFFu
#define CRC_8_XOR_VALUE                     0xFFu
#define CRC_8_MODE							RUNTTIME

/* ---------- Defines for 16-bit CCITT CRC calculation (Not reflected) ---------------------------------------------------------- */
#define CRC_16_RESULT_WIDTH                 16u
#define CRC_16_POLYNOMIAL                   0x1021u
#define CRC_16_INIT_VALUE                   0xFFFFu
#define CRC_16_XOR_VALUE                    0x0000u
#define CRC_16_MODE							TABLE

/* ---------- Defines for 32-bit CCITT CRC calculation (Reflected) -------------------------------------------------------------- */
#define CRC_32_RESULT_WIDTH                 32u
#define CRC_32_POLYNOMIAL                   0x04C11DB7u
#define CRC_32_INIT_VALUE                   0xFFFFFFFFu
#define CRC_32_XOR_VALUE                    0xFFFFFFFFu
#define CRC_32_MODE							RUNTTIME


/**
 * @brief
 */
void sm_CRC_Init(void);

/**
 * @brief
 * @param Buffer
 * @param Length
 * @return
 */
uint8_t sm_CRC_CalculateCRC8(const uint8_t *Buffer, uint16_t Length);
/**
 * @brief
 * @param Buffer
 * @param Length
 * @return
 */
uint16_t sm_CRC_CalculateCRC16(const uint8_t *Buffer, uint32_t Length);

/**
 * @brief
 * @param Buffer
 * @param Length
 * @return
 */
uint32_t sm_CRC_CalculateCRC32(const uint8_t *Buffer, uint32_t Length);


/**
 * @brief
 * @param init value
 * @param Buffer
 * @param Length
 * @return
 */
uint16_t sm_CRC_CalculateCRC16_init_value(uint16_t crc_init, const uint8_t *Buffer, uint32_t Length);



#ifdef __cplusplus
}
#endif

#endif /* CRC_H_ */
