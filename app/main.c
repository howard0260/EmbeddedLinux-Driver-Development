#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "../device/led.h"
#include "../device/button.h"
#include "../device/ads1115.h"

#define LED_PIN         9
#define BUTTON_PIN      17
#define I2C_BUS         1
#define ADS1115_ADDR  0x48
#define MODE_IDLE       0
#define MODE_AUTO       1
#define MODE_MANNUAL    2


LED_t led;
BUTTON_t button;
ADS1115_t ads1115;

char sys_mode = MODE_IDLE;
int running = 1;
void led_blink(LED_t *led); 


// Mutex protect sys_mode
pthread_mutex_t mode_lock = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int signal)
{
        printf("signal:%d\n",signal);
        if (signal == SIGINT)
        {
            printf("Process End\n");
            running = 0;
        }
}

// Thread 1：Read the raw data from ADS1115 automatically
void *auto_read_task(void *arg)
{
    int16_t buf = 0;
    float voltage = 0;
    while(running) {
        if (sys_mode == MODE_AUTO)
        {
            // 讀取 Channel 0
            if(ads1115_read_raw(&ads1115,ADS1115_CHANNEL_0,&buf) < 0)
            {
                printf("read raw data Failed\n");
                return NULL;
            }
            // LED 閃一下
            led_blink(&led);
            // printf 結果
            printf("raw data result is %d\n",buf);
            // sleep(1)
            usleep(10000);
        }
        else if (sys_mode == MODE_MANNUAL)
        {
            if(ads1115_read_raw(&ads1115,ADS1115_CHANNEL_0,&buf) < 0)
            {
                printf("read buf failed\n");
            }
            else{
                voltage = ads1115_to_voltage(&ads1115,buf);
                led_blink(&led);
                printf("[Manual] buf:%d voltage:%.4fV\n",buf,voltage);
            }
            /// mutex lock
            pthread_mutex_lock(&mode_lock);
            sys_mode = MODE_IDLE;
            pthread_mutex_unlock(&mode_lock);
        }
        else{
            usleep(100000);
        }
    }
    return NULL;
}

// Thread 2：Button monitor
void *button_task(void *arg)
{
    int press_time = 0;
    while(running) {
        // Read Button state
        BUTTON_Status btnstate = button_status_read(&button);
        if ( btnstate == PRESSED)
        {
            press_time++;
            usleep(100000);
        }
        else if (btnstate == RELEASED && press_time > 0)
        {
            if(press_time >= 10)
            {
                sys_mode = MODE_AUTO;
            }
            else{
                sys_mode = MODE_MANNUAL;
            }
            press_time = 0;
            
        }
        // 按下時做一次轉換
        usleep(100000);
    }
    return NULL;
}

void led_blink(LED_t *led)
{
    led_on(led);
    usleep(200000);  // 200ms
    led_off(led);
}

int main()
{
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
    /// I2c Init
    if(ads1115_init(&ads1115 ,I2C_BUS ,ADS1115_ADDR, ADS1115_SINGLESHOT, ADS1115_SPS_64, ADS1115_PGA_2048) < 0)
    {
        printf("ADS1115 init Filed\n");
        return -1;
    }
    /// Create 2 thread
    pthread_t t1,t2;
	pthread_create(&t1, NULL, auto_read_task, NULL);
	pthread_create(&t2, NULL, button_task, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

    // clear after end
    // turn off LED
    led_off(&led);
    // release the resource
    led_deinit(&led);
    button_deinit(&button);
    ads1115_deinit(&ads1115);
    printf("Program End\n");

    return 0;
}














