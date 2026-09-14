#include <stdio.h>
#include "led.h"
#include "../hal/gpio.h"


int led_init(LED_t *led,unsigned int pin)
{
    if(gpio_init(&led->gpio,
                  0,
                  pin,
                  GPIO_DIR_OUTPUT,
                  "led") != 0)
    {
        return -1;
    }
    led->state = OFF;
    return 0;
}

int led_on(LED_t *led)
{
    if ( gpio_write(&led->gpio,GPIO_HIGH) != 0x0)
        return -1;
    
    led->state = ON;
    return 0;

}
int led_off(LED_t *led)
{
    if ( gpio_write(&led->gpio,GPIO_LOW) != 0x0)
        return -1;
    led->state = OFF;
    return 0;
}


int led_toggle(LED_t *led)
{
    if(led->state == OFF)
        return led_on(led);
    else
        return led_off(led);

}

LED_Status led_status_read(LED_t *led)
{
    return led->state;
}

void led_deinit(LED_t *led)
{
    led_off(led);
    gpio_deinit(&led->gpio);
}


