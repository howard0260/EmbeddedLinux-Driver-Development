#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef struct
{
  int      fd;       // 檔案描述符（/dev/spidev0.0）
  uint8_t  mode;
  uint8_t  bits;
  uint32_t speed; 
} SPI_t;

int spi_init(SPI_t *spi, uint8_t bus,uint8_t cs,uint8_t mode, uint8_t bits,uint32_t speed);
int spi_transfer(SPI_t *spi,uint8_t *wdata,uint8_t *rdata, uint32_t datasize);
void spi_deinit(SPI_t *spi);

#endif