#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>            // open()
#include <unistd.h>           // close()
#include <sys/ioctl.h>        // ioctl()
#include <linux/spi/spidev.h> // SPI_IOC_MESSAGE
#include "spi.h"


int spi_init(SPI_t *spi, uint8_t bus,uint8_t cs,uint8_t mode, uint8_t bits,uint32_t speed)
{
    char spi_path[32];
    snprintf(spi_path,sizeof(spi_path),"/dev/spidev%d.%d",bus,cs);
    spi->fd = open(spi_path,O_RDWR);
    if (spi->fd < 0)
    {
        printf("spi initial Failed\n");
        return -1;
    }
    if(ioctl(spi->fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        printf("Failed to set SPI mode\n");
        /// close fd
        close(spi->fd);
        return -1;
    }
    spi->mode = mode;
    spi->bits = bits;
    spi->speed = speed;

    if (ioctl(spi->fd,SPI_IOC_WR_BITS_PER_WORD,&bits) < 0){
        printf("Failed to set SPI bits per word\n");
        close(spi->fd);
        return -1;
    }

    if (ioctl(spi->fd,SPI_IOC_WR_MAX_SPEED_HZ &speed) < 0){
        printf("Failed to set SPI speed\n");
        close(spi->fd);
        return -1;
    }

    return 0;
}

int spi_transfer(SPI_t *spi,uint8_t *wdata,uint8_t *rdata, uint32_t datasize)
{        
    if (!spi || spi->fd < 0) return -1;
    if (!wdata ||!rdata || datasize == 0) return -1;
    ///// It's important as below because the sturcture defined by #include <linux/spi/spidev.h>
    struct spi_ioc_transfer tr = {
        .tx_buf        = (unsigned long)wdata,
        .rx_buf        = (unsigned long)rdata,
        .len           = datasize,
        .speed_hz      = spi->speed,
        .bits_per_word = spi->bits,
        .delay_usecs   = 0,
    };
    if (ioctl(spi,SPI_IOC_MESSAGE(1), &tr) < 0)
    {
        printf("SPI transfer failed\n");
        return -1;
    }
    return 0;
}

void spi_deinit(SPI_t *spi)
{
    if (!spi) return;
    if(spi->fd >= 0)
    {
        close(spi->fd);
        spi->fd = -1;
    }
}
