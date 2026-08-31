/* tlv320.h - Generic TLV320 I2C programmer
 */
#ifndef __TLV320_H__
#define __TLV320_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t reg;
  uint8_t val;
} TLV320_Reg_t;

/**
 * Program a list of register writes to a TLV320-style codec over I2C.
 *
 * @param hi2c Pointer to the I2C handle (e.g. &hi2c2)
 * @param DevAddress 7-bit device address shifted left by 1 (HAL convention)
 * @param list Array of register/value pairs to write
 * @param len Number of entries in @p list
 * @param Timeout HAL timeout in ms for each write
 * @return HAL status (HAL_OK on success)
 */
HAL_StatusTypeDef TLV320_Program(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, const TLV320_Reg_t *list, size_t len, uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif /* __TLV320_H__ */
