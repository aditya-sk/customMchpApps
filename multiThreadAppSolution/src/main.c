#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>

// Application header with structure and function prototype
#include <app_pwm_thread.h>
#include <app_button_thread.h>
#include <app_sensor_thread.h>

// bno055 library header file
#include <bno055.h>


LOG_MODULE_REGISTER(main_log, LOG_LEVEL_INF);

// TODO #1: define stack size
#define STACKSIZE 512
#define STACKSIZE_I2C 1024

// define priority for each threads 
#define PRIORITY_BUTTON    3
#define PRIORITY_PWM    4
#define PRIORITY_I2C    5



// TODO #2: stack size defined to each specific thread
K_THREAD_STACK_DEFINE(blinkThreadstack_area, STACKSIZE);
struct k_thread blinkThread;
K_THREAD_STACK_DEFINE(sampledDataThreadstack_area, STACKSIZE_I2C);
struct k_thread sampledDataThread;
K_THREAD_STACK_DEFINE(buttonThreadstack_area, STACKSIZE);
struct k_thread buttonThread;


// TODO #3: creating, and allocating the stack size for each thread and priority in the system
int main(void)
{
        initSYSTEM();
        
        k_tid_t button_tid = k_thread_create(&buttonThread, buttonThreadstack_area,
                                 K_THREAD_STACK_SIZEOF(buttonThreadstack_area),
                                 channelSelect_Thread,
                                 NULL, NULL, NULL,
                                 PRIORITY_BUTTON, 0, K_NO_WAIT);
        k_tid_t blink_tid = k_thread_create(&blinkThread, blinkThreadstack_area,
                                 K_THREAD_STACK_SIZEOF(blinkThreadstack_area),
                                 ledPWM_Thread,
                                 NULL, NULL, NULL,
                                 PRIORITY_PWM, 0, K_NO_WAIT);
        k_tid_t slave_tid = k_thread_create(&sampledDataThread, sampledDataThreadstack_area,
                                 K_THREAD_STACK_SIZEOF(sampledDataThreadstack_area),
                                 fetchGet_Thread,
                                 NULL, NULL, NULL,
                                 PRIORITY_I2C, 0, K_NO_WAIT);

        k_thread_name_set(button_tid, (const char *)"ButtonThread");
        k_thread_name_set(blink_tid, (const char *)"BlinkThread");
        k_thread_name_set(slave_tid, (const char *)"BNO055Thread");
        
        return 0;
}

