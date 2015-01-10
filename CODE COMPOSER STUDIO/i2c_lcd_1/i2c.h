#ifndef I2C_H
#define I2C_H 1

#include "common.h"

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write8(uint8_t c);
uint8_t i2c_read8(uint8_t acknack);

#endif

