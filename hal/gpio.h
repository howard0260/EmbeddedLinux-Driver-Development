#ifndef GPIO_H
#define GPIO_H

#include <gpiod.h>


// #define GPIOD_LINE_VALUE_INACTIVE 1 // gpiod.h has itself define

typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH
}GPIO_Status;

typedef enum {
    GPIO_BIAS_DISABLE   = 0,
    GPIO_BIAS_PULL_UP   = 1,
    GPIO_BIAS_PULL_DOWN = 2,
} GPIO_BIAS;

typedef enum
{
    GPIO_DIR_OUTPUT = 0,
    GPIO_DIR_INPUT
}GPIO_DIRECTION;

typedef struct
{
    struct gpiod_chip *chip;
    // struct gpiod_line *line; //V1
    struct gpiod_line_request *request;  // V2
    GPIO_DIRECTION direction;
    unsigned int pin;

}GPIO_t;

int gpio_init(GPIO_t *gpio,unsigned int chip_num,unsigned int pin,GPIO_BIAS bias,GPIO_DIRECTION direction,const char *consumer);
int gpio_write(GPIO_t *gpio,GPIO_Status value);
int gpio_read(GPIO_t *gpio);
void gpio_deinit(GPIO_t *gpio);

#endif