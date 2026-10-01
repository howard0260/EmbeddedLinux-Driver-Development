#ifndef ADS1115_H
#define ADS1115_H
#include "../hal/i2c.h"
#include <stdint.h>

#define ADS1115_ADDR    0x48


typedef enum
{
    ADS1115_REG_CONVERSION = 0,
    ADS1115_REG_CONFIG = 1,
    ADS1115_REG_LO_THRESH,
    ADS1115_REG_HI_THRESH
}ADS1115_Register;

typedef enum {
    ADS1115_CHANNEL_0 = 4,  // AIN0
    ADS1115_CHANNEL_1 = 5,  // AIN1
    ADS1115_CHANNEL_2 = 6,  // AIN2
    ADS1115_CHANNEL_3 = 7,  // AIN3
} ADS1115_Channel;

typedef enum
{
    ADS1115_CONTINUOUS = 0,
    ADS1115_SINGLESHOT = 1
}ADS1115_Mode;

typedef enum
{
    ADS1115_SPS_8 = 0,
    ADS1115_SPS_16 = 1,
    ADS1115_SPS_32,
    ADS1115_SPS_64,
    ADS1115_SPS_128,
    ADS1115_SPS_250,
    ADS1115_SPS_475,
    ADS1115_SPS_860
}ADS1115_DR;

typedef enum {
    ADS1115_PGA_6144 = 0,  // ±6.144V
    ADS1115_PGA_4096 = 1,  // ±4.096V
    ADS1115_PGA_2048 = 2,  // ±2.048V（default）
    ADS1115_PGA_1024 = 3,  // ±1.024V
    ADS1115_PGA_0512 = 4,  // ±0.512V
    ADS1115_PGA_0256 = 5,  // ±0.256V
} ADS1115_PGA;

typedef struct{
    I2C_t i2c;
    ADS1115_Mode mode;
    ADS1115_DR datarate;
    ADS1115_PGA pga;
}ADS1115_t;

int ads1115_init(ADS1115_t *ads1115,uint8_t bus_number,uint8_t addr,ADS1115_Mode mode,ADS1115_DR dr,ADS1115_PGA pga);
int ads1115_read_raw(ADS1115_t *ads1115,uint8_t channel,int16_t *rdata);
float ads1115_to_voltage(ADS1115_t *ads1115,uint16_t rdata);
// int ads1115_register_write(ADS1115_t *ads1115,uint8_t channel,int16_t *wdata);
void ads1115_deinit(ADS1115_t *ads1115);


#endif
