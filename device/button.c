#include <stdio.h>
#include "button.h"


int button_init(BUTTON_t *button,unsigned int pin)
{
    if(gpio_init(&button->gpio,
                  0,
                  pin,
                  GPIO_BIAS_PULL_UP,
                  GPIO_DIR_INPUT,
                  "button") != 0)
    {
        return -1;
    }
    button->state = RELEASED;
    return 0;
}


BUTTON_Status button_status_read(BUTTON_t *button)
{
    int val = gpio_read(&button->gpio);
    printf("gpio_read = %d\n", val);
    button->state = (val == GPIO_LOW) ? PRESSED:RELEASED;
    return button->state;
}

void button_deinit(BUTTON_t *button)
{
    gpio_deinit(&button->gpio);
}


