#include <stdio.h>
#include "gpio.h"


struct gpiod_chip_info *info;

int gpio_init(GPIO_t *gpio,unsigned int chip_num,unsigned int pin,GPIO_DIRECTION direction,const char *consumer)
{
    char chip_path[32];
    snprintf(chip_path,sizeof(chip_path),"/dev/gpiochip%d",chip_num);
    gpio->pin = pin;
    gpio->direction = direction;
    /// Step 1 open chip
    gpio->chip = gpiod_chip_open(chip_path);
    if(!gpio->chip)
    {
        printf("Failed to open GPIO chip\r\n");
        return -1;
    }

    /// Step 2 create line_settings
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if(!settings)
    {
        printf("Failed to create line settings\n");
        gpiod_chip_close(gpio->chip);
        return -1;
    }
    /// Step 3 setup the direction
    if (direction == GPIO_DIR_OUTPUT)
    {
        if(gpiod_line_settings_set_direction(settings,GPIOD_LINE_DIRECTION_OUTPUT) < 0 )
        {
            printf("Failed to request GPIO%d as output\n", pin);
            gpiod_line_settings_free(settings);
            gpiod_chip_close(gpio->chip);
            return -1;
        }
        if(gpiod_line_settings_set_output_value(settings,GPIOD_LINE_VALUE_INACTIVE) < 0)
        {
            printf("Failed to set output GPIO%d value\n", pin);
            gpiod_line_settings_free(settings);
            gpiod_chip_close(gpio->chip);
            return -1;
        }
    }
        
    else if (direction == GPIO_DIR_INPUT)
    {
        if(gpiod_line_settings_set_direction(settings,GPIOD_LINE_DIRECTION_INPUT) < 0)
        {
            printf("Failed to request GPIO%d as input\n", pin);
            gpiod_line_settings_free(settings);
            gpiod_chip_close(gpio->chip);
            return -1;  
        }
    }
    /// Step 4 create line_config
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if(!line_cfg)
    {
        printf("Failed to create line config\n");
        /// Need to free and close the gpio
        gpiod_line_settings_free(settings);
        gpiod_chip_close(gpio->chip);
        return -1;
    }
    /// Step 5 Apply settings to a specified footer
    if(gpiod_line_config_add_line_settings(line_cfg,&pin,1,settings))
    {
        printf("Failed to add line settings\n");
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        gpiod_chip_close(gpio->chip);
        return -1;
    }
    /// Step 6 Create request_config
    struct gpiod_request_config *request_cfg = gpiod_request_config_new();
    if(!request_cfg)
    {
        printf("Failed to create request config\n");
        /// Need to free the settings and line_cfg
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        gpiod_chip_close(gpio->chip);
        return -1;
    }
    /// Step 7 Setup consumer name
    gpiod_request_config_set_consumer(request_cfg,consumer);
    /// Step 8 Send a request to gain control
    gpio->request = gpiod_chip_request_lines(gpio->chip,request_cfg,line_cfg);
    if(!gpio->request)
    {
        printf("Failed to request GPIO line %d\n", pin);
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        gpiod_request_config_free(request_cfg);
        gpiod_chip_close(gpio->chip);
        return -1;
    }
    /// Step 9 Release config files that no longer needed
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(request_cfg);
    
        
    return 0;
}

int gpio_write(GPIO_t *gpio,GPIO_Status value)
{
    enum gpiod_line_value val =
        (value == GPIO_HIGH) ? GPIOD_LINE_VALUE_ACTIVE
                             : GPIOD_LINE_VALUE_INACTIVE;
    return gpiod_line_request_set_value(gpio->request,gpio->pin,val);
}

int gpio_read(GPIO_t *gpio)
{
    enum gpiod_line_value val =
        gpiod_line_request_get_value(gpio->request, gpio->pin);
    return (val == GPIOD_LINE_VALUE_ACTIVE) ? GPIO_HIGH : GPIO_LOW;
}

void gpio_deinit(GPIO_t *gpio)
{
    if(gpio->request)
    {
        gpiod_line_request_release(gpio->request);
    }
    if (gpio->chip)
    {
        gpiod_chip_close(gpio->chip);
    }
    
}
