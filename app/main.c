#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "../device/led.h"
#include "../device/button.h"

#define LED_PIN 9
#define BUTTON_PIN 17
int running = 1;

void signal_handler(int signal)
{
        printf("signal:%d\n",signal);
        if (signal == SIGINT)
        {
            printf("Process End\n");
            running = 0;
        }
}


int main()
{
    LED_t led;
    BUTTON_t button;
    // int status = 0;
    // register the signal handler
    signal(SIGINT, signal_handler);
    // LED init
    if(led_init(&led,LED_PIN) < 0)
    {
        printf("LED init failed\n");
        return -1;
    }
    // Button Init
    if(button_init(&button,BUTTON_PIN) < 0)
    {
        printf("Button init failed\n");
        return -1;
    }
    while(running)
    {
        BUTTON_Status btnstate = button_status_read(&button);
        if ( btnstate == PRESSED)
        {
            led_on(&led);
        }
        else{
            led_off(&led);
        }
        printf("Button: %s LED: %s\n",
           btnstate == PRESSED ? "PRESSED" : "RELEASED",
           led_status_read(&led) == ON ? "ON" : "OFF");
        // wait for 100ms
        usleep(100000);
    }

    // clear after end
    // turn off LED
    led_off(&led);
    // release the resource
    led_deinit(&led);
    button_deinit(&button);

    return 0;
}














