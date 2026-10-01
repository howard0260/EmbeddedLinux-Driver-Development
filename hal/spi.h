#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>          // open()
#include <unistd.h>         // read(), write(), close()
#include <sys/ioctl.h>      // ioctl()
#include <linux/i2c-dev.h>  // I2C_SLAVE
#include <string.h>
#include "spi.h"