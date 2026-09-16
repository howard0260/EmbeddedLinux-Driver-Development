#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>          // open()
#include <unistd.h>         // read(), write(), close()
#include <sys/ioctl.h>      // ioctl()
#include <linux/i2c-dev.h>  // I2C_SLAVE
#include <string.h>
#include "i2c.h"




int i2c_init(I2C_t *i2c, uint8_t bus_num, uint8_t address)
{
    char i2c_path[32];
    snprintf(i2c_path,sizeof(i2c_path),"/dev/i2c-%d",bus_num);
    i2c->fd = open(i2c_path,O_RDWR);
    if (i2c->fd < 0)
    {
        printf("i2c initial Failed\n");
        return -1;
    }
    if(ioctl(i2c->fd, I2C_SLAVE, address) < 0)
    {
        printf("Failed to set I2C slave address 0x%02X\n", address);
        /// close fd
        close(i2c->fd);
        return -1;
    }
    i2c->bus_num = bus_num;
    i2c->addr = address;
    return 0;
}

int i2c_write(I2C_t *i2c, uint8_t *wdata, uint32_t wdatasize)
{
    if (!i2c || i2c->fd < 0) return -1;
    if (!wdata || wdatasize == 0) return -1;
    ssize_t ret = write(i2c->fd,wdata,wdatasize);
    if (ret != (ssize_t)wdatasize)
    {
        printf("I2C Write Failed\n");
        return -1;
    }
    return 0;
}

int i2c_read(I2C_t *i2c,uint8_t *rdata, uint32_t rdatasize)
{
    if (!i2c || i2c->fd < 0) return -1;
    if (!rdata || rdatasize == 0) return -1;
    ssize_t ret = read(i2c->fd,rdata,rdatasize);
    if (ret != (ssize_t)rdatasize)
    {
        printf("I2C Read Failed\n");
        return -1;
    }
    return 0;
}

int i2c_write_read(I2C_t *i2c,uint8_t *wdata, uint32_t wdatasize, uint8_t *rdata, uint32_t rdatasize)
{
    if (!i2c || i2c->fd < 0) return -1;
    if (!wdata || wdatasize == 0 || !rdata || rdatasize == 0) return -1;


    if (i2c_write(i2c,wdata,wdatasize) < 0)
    {
        printf("I2C Write Failed\n");
        return -1;
    }
    if (i2c_read(i2c,rdata,rdatasize) < 0)
    {
        printf("I2C Read Failed\n");
        return -1;
    }

    return 0;
}

int i2c_write_register(I2C_t *i2c,uint8_t reg,uint8_t *wdata,uint32_t wdatasize)
{
    if (!i2c || i2c->fd < 0) return -1;
    if (!wdata || wdatasize == 0) return -1;
    uint8_t buf[wdatasize + 1];
    buf[0] = reg;
    if (wdatasize > 0)
    {
        memcpy(&buf[1],wdata,wdatasize);
    }

    ssize_t ret = write(i2c->fd,buf,wdatasize + 1);
    if(ret != (ssize_t)(wdatasize + 1))
    {
        printf("I2C Write Register Failed\n");
        return -1;
    }
    return 0;
}

int i2c_read_register(I2C_t *i2c,uint8_t reg,uint8_t *rdata,uint32_t rdatasize)
{
    if (!i2c || i2c->fd < 0) return -1;
    if (!rdata || rdatasize == 0) return -1;


}

void i2c_deinit(I2C_t *i2c)
{
    /// need to confirm if i2c->fd is open; open-> close, but the status is close, don't need do anything
    if(i2c->fd >= 0)
    {
        close(i2c->fd);
        i2c->fd = -1; /// important!!!! to prevent closing other fd
    }
}