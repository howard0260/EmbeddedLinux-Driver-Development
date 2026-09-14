#ifndef BUTTON_H
#define BUTTON_H
#include "../hal/gpio.h"

typedef enum
{
    RELEASED = 0,
    PRESSED = 1
}BUTTON_Status;

typedef struct{
    GPIO_t gpio;
    BUTTON_Status  state; 
}BUTTON_t;

int button_init(BUTTON_t *button,unsigned int pin);
void button_deinit(BUTTON_t *button);
BUTTON_Status button_status_read(BUTTON_t *button);

#endif