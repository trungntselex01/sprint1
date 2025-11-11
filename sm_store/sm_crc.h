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

#define RUNTTIME                            0
#define TABLE                               1
#define HARDWARE                            2

#define CRC_16_RESULT_WIDTH                 16u
#define CRC_16_POLYNOMIAL                   0x1021u
#define CRC_16_INIT_VALUE                   0xFFFFu
#define CRC_16_XOR_VALUE                    0x0000u
#define CRC_16_MODE                         TABLE

/* ---------- Defines for 32-bit CCITT CRC calculation (Reflected) -------------------------------------------------------------- */
#define CRC_32_RESULT_WIDTH                 32u
#define CRC_32_POLYNOMIAL                   0x04C11DB7u
#define CRC_32_INIT_VALUE                   0xFFFFFFFFu
#define CRC_32_XOR_VALUE                    0xFFFFFFFFu
#define CRC_32_MODE                         RUNTTIME

uint16_t sm_CRC_CalculateCRC16(const uint8_t *Buffer, uint32_t Length);

/**
 * @brief
 * @param Buffer
 * @param Length
 * @return
 */
uint32_t sm_CRC_CalculateCRC32(const uint8_t *Buffer, uint32_t Length);

#endif /* SM_CRC_H_ */
