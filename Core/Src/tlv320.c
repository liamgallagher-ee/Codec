/* tlv320.c - Generic TLV320 I2C programmer
 */
#include "tlv320.h"

HAL_StatusTypeDef TLV320_Program(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, const TLV320_Reg_t *list, size_t len, uint32_t Timeout)
{
  if (hi2c == NULL || list == NULL) return HAL_ERROR;

  for (size_t i = 0; i < len; ++i) {
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(hi2c, DevAddress, list[i].reg, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&list[i].val, 1, Timeout);
    if (ret != HAL_OK) return ret;
    HAL_Delay(1);
  }

  return HAL_OK;
}

/* Example placeholder array you can populate from "Programming_TLV320_Made_Easy".
   Fill this with the register/value pairs from the guide and call TLV320_Program(&hi2c2, 0x1A<<1, tlv320_init_cmds, ARRAY_SIZE(tlv320_init_cmds), 100);
*/
 const TLV320_Reg_t tlv320_init_cmds[] = {
  {0x01, 0x08}, //reset codec
  {0x13, 0x04}, //Route Line1LP to left ADC, power Up
  {0x16, 0x04}, //Route Line1RP to right ADC, power Up
  {0x0F, 0x00}, //Unmute Left PGA, set gain to 0dB
  {0x10, 0x00}, //Unmute Right PGA, set gain to 0dB
  {0x07, 0x0A}, //Route Left data to Left DAC, Route Right Data to Right DAC
  {0x25, 0xC0}, //Power up DACs
  {0x2B, 0x00}, //Unmute digital volume control, set gain to 0dB
  {0x2C, 0x00}, //Set digital volume to 0dB
  {0x52, 0x80}, //route Left DAC to left line out
  {0x5C, 0x80}, //route Right DAC to right line out
  {0x56, 0x09}, //power up left line out differential pair, set gain to 0dB
  {0x5D, 0x00} //power up right line out differential pair, set gain to 0dB
};
