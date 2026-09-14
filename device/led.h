#ifndef LED_H
#define LED_H
#include "gpio.h"

typedef enum
{
    OFF = 0,
    ON = 1
}LED_Status;

typedef struct{
    GPIO_t gpio;
    LED_Status  state; 
}LED_t;

int led_init(LED_t *led,unsigned int pin);
int led_on(LED_t *led);
int led_off(LED_t *led);
int led_toggle(LED_t *led);
void led_deinit(LED_t *led);
LED_Status led_status_read(LED_t *led);


#endif
