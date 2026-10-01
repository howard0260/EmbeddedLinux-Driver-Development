#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ads1115.h"

static const uint16_t channel_config[4] = {
    0xC183,  // AIN0
    0xD183,  // AIN1
    0xE183,  // AIN2
    0xF183,  // AIN3
};

static const float pga_voltage[6] = {
    6.144f / 32768.0f,  // PGA_6144
    4.096f / 32768.0f,  // PGA_4096
    2.048f / 32768.0f,  // PGA_2048
    1.024f / 32768.0f,  // PGA_1024
    0.512f / 32768.0f,  // PGA_0512
    0.256f / 32768.0f,  // PGA_0256
};

int ads1115_init(ADS1115_t *ads1115,uint8_t bus_number,uint8_t addr,ADS1115_Mode mode,ADS1115_DR dr,ADS1115_PGA pga)
{
    if(!ads1115)
        return -1;
    if(i2c_init(&ads1115->i2c, bus_number, addr) < 0)
    {
        printf("ads1115 initial Filed\n");
        return -1;
    }
    ads1115->mode = mode;
    ads1115->datarate = dr;
    ads1115->pga = pga;
    return 0;
}

int ads1115_read_raw(ADS1115_t *ads1115,uint8_t channel,int16_t *rdata)
{
    if (!ads1115 || !rdata)   return -1;
    // Step 1: Configuration register settings per datasheet specification
    uint16_t config = channel_config[channel];
        
    /// mux[14:12]
    static const uint8_t mux_config[] = {
        0x04,  // AIN0 → MUX = 100
        0x05,  // AIN1 → MUX = 101
        0x06,  // AIN2 → MUX = 110
        0x07,  // AIN3 → MUX = 111
    };
    config &= ~(0x7000);    // clear the mux bit to 0
    config |= (mux_config[channel] << 12);
    /// pga[11:9]
    config &= ~(0x0E00);    // clear the pga digits[9-11] to zero
    config |= (ads1115->pga << 9);
    /// mode[8]
    config &= ~(0x0100);
    config |= (ADS1115_SINGLESHOT << 8); //0:continuous/1:one-shot
    /// dr[7:5]
    config &= ~(0x00E0);    // clear the dr digits[5-7] to zero
    config |= (ads1115->datarate << 5);
    /// comp_que[1:0]
    config &= ~(0x0003);    // clear the comp_que digits[0-1] to zero
    config |= (0x03);
    // OS[15]
    config |= (1 << 15);
    // Step 2: write to config pointer register
    uint8_t config_data[2];
    config_data[0] = (config >> 8) & 0xFF;  // MSB
    config_data[1] = config & 0xFF;  
    if (i2c_write_register(&ads1115->i2c,ADS1115_REG_CONFIG,config_data,2) < 0)
    {
        printf("Config register write failed\n");
        return -1;
    }
     // Step 3: wait for conversion complete
    usleep(8000);
     // Step 4: read conversion register
     uint8_t buf[2] = {0};
    if(i2c_read_register(&ads1115->i2c,ADS1115_REG_CONVERSION,buf,2) < 0)
    {
        printf("ads1115 read raw data failed\n");
        return -1;
    }
    *rdata = (buf[0] << 8) | buf[1];
    return 0;
}

float ads1115_to_voltage(ADS1115_t *ads1115,uint16_t rdata)
{
    if (!ads1115)
    {
        return 0.0f;
    }
    return (float)pga_voltage[ads1115->pga]*rdata;
}

// int ads1115_register_write(ADS1115_t *ads1115,uint8_t channel,int16_t *wdata);
void ads1115_deinit(ADS1115_t *ads1115)
{
    i2c_deinit(&ads1115->i2c);
}


