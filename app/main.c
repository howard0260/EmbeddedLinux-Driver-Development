#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "../device/led.h"

#define LED_PIN 9
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
    int status = 0;
    // register the signal handler
    signal(SIGINT, signal_handler);
    // LED init
    if(led_init(&led,LED_PIN) < 0)
    {
        printf("LED init failed\n");
        return -1;
    }
    while(running)
    {
        // switch LED
        led_toggle(&led);
        // print the status
        status = led_status_read(&led);
        printf("LED status:%d\r\n",status);
        // wait for 1 second
        sleep(1);
    }

    // clear after end
    // turn off LED
    led_off(&led);
    // release the resource
    led_deinit(&led);

    return 0;
}














