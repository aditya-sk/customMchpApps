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




// TODO #2: stack size defined to each specific thread



// TODO #3: creating, and allocating the stack size for each thread and priority in the system
int main(void)
{
        initSYSTEM();
        

        
        return 0;
}

