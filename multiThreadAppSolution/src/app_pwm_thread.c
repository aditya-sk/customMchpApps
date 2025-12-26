
// zephyr library header files
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


LOG_MODULE_REGISTER(pwm_thread_log, LOG_LEVEL_INF);

struct pwm_dt_spec pwmBlink  = PWM_DT_SPEC_GET(DT_ALIAS(userled));


// initialize ledPWM, swo and i2c node connected with BNO055 sensor
void initSYSTEM()
{
        
        if(!pwm_is_ready_dt(&pwmBlink))
        {
            LOG_ERR("Error occure while PWM init\r\n");
        }
        if(0 != init_SW0())
        {
            LOG_ERR("Error occure while GPIO init\r\n");
        }
        initI2CNode();
        
}


// TODO #5: thread to set the pwm pulse and disable it

void ledPWM_Thread(void* arg1, void* arg2, void* arg3)
{
    int ret;

        while(1)
        {
                LOG_INF("Period %ld msec\n", MAX_PERIOD/1000000);
                ret = pwm_set_dt(&pwmBlink, MAX_PERIOD, MAX_PERIOD / PERIOD_HALF_DIVISOR);
                if(ret)
                {
                    LOG_ERR("Error %d: failed to set pulse width in state S1\n", ret);
                }
        pwm_set_dt(&pwmBlink, 0, 0);
        // stays in blocked state for 6 seconds
        k_sleep(TIMEOUT_SIX_SEC);  

        }
        
}



