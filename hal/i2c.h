#ifndef I2C_H
#define I2C_H
#include <stdio.h>
#include <stdint.h>

typedef struct
{
  int      fd;       // 檔案描述符（/dev/i2c-x）
  uint8_t  addr;     // slave address（例如 ADS1115 = 0x48）
   uint8_t  bus_num;  // I2C bus 號碼（1 = /dev/i2c-1）
} I2C_t;

int i2c_init(I2C_t *i2c, uint8_t bus_num, uint8_t address);
int i2c_write(I2C_t *i2c, uint8_t *wdata, uint32_t wdatasize);
int i2c_read(I2C_t *i2c,uint8_t *rdata, uint32_t rdatasize);
int i2c_write_read(I2C_t *i2c,uint8_t *wdata, uint32_t wdatasize, uint8_t *rdata, uint32_t rdatasize);
int i2c_write_register(I2C_t *i2c,uint8_t reg,uint8_t *wdata,uint32_t wdatasize);
int i2c_read_register(I2C_t *i2c,uint8_t reg,uint8_t *rdata,uint32_t rdatasize);
void i2c_deinit(I2C_t *i2c);

#endif