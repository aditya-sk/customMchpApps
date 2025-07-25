// Zephyr library header files
#include <zephyr/kernel.h>
#include <zephyr/smf.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/reboot.h>



// Application header with Enums, Structure and function prototype
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>

// sensor library header file
#include <bno055.h>

LOG_MODULE_REGISTER(pwmLED_LOG);

// TODO #1: get the devicetree node
struct pwm_dt_spec pwmBlink  = PWM_DT_SPEC_GET(DT_ALIAS(userled));



// TODO #: Checking if the PWM is ready or not 
void init_ENTRY(void* ptrData) 
{
        LOG_INF("Init Peripherals\r\n");
        
        if(0 != initI2CNode()){
            
            LOG_ERR("Error occured while I2C init\r\n");
        }
        if(true != pwm_is_ready_dt(&pwmBlink))
        {
            LOG_ERR("Error occured while PWM init\r\n");
        
        }
        if(0 != init_SW0())
        {
            LOG_ERR("Error occured while GPIO init\r\n");

        }
        
        k_sleep(TIMEOUT_FOUR_SEC); 
}


//TODO #: Set the parameter to zero
enum smf_state_result init_RUN(void* ptrData)
{

        pwm_set_dt(&pwmBlink, PERIOD_NS, PERIOD_NS);  //(0 duty cycle)
        LOG_INF("Welcome to HSM State Machines\r\n");
        k_sleep(TIMEOUT_TWO_SEC); 
        smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[LED_BLINK]); // Transition to caliberation mode
        return SMF_EVENT_HANDLED;
}

//TODO #: Exit from init PWM state
void init_EXIT(void* ptrData)
{
        LOG_INF("Completed Init for I2C, PWM and GPIO Peripherals\r\n");
}


enum smf_state_result ledPWM_RUN(void* ptrData)
{
    int ret;
        struct FSMContext* info = (struct FSMContext*)ptrData;
        if(info->events & EVENT_BTN_PRESS){
            return SMF_EVENT_PROPAGATE;
        }else{
            // We have to blink led and move to sensor state
            for(int count = 0; count < 3; count++){
            ret = pwm_set_dt(&pwmBlink, PERIOD_NS, PERIOD_NS / PERIOD_DIVISOR);
            if(ret)
            {
                LOG_ERR("Error %d: failed to set pulse width in state S1\n", ret);
                smf_set_state(SMF_CTX(info), &stateTable[ERROR_STATE]); // Transition to S0 in case of error
            }
            k_sleep(TIMEOUT_TWO_SEC); // Stay in S1 for 4 seconds before transitioning
            }

        smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[SAMPLE_FORMAT]); // Transition to S2 (fast blink)
        return SMF_EVENT_HANDLED;
        
        }
}

void ledPWM_EXIT(void* ptrData)
{
    
    pwm_set_dt(&pwmBlink, PERIOD_NS, 0);
    LOG_INF("PWM Blink Completed\r\n");
}


// Error State has only error actions
enum smf_state_result errorHandle_RUN(void* ptrData)
{
    const struct FSMContext* info = (const struct FSMContext*)ptrData;

        if (info->ctx.previous == &stateTable[LED_BLINK] || info->ctx.previous == &stateTable[SAMPLE_FORMAT]) 
        {
                smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[INITIALIZE]);
                return SMF_EVENT_HANDLED;
        }
        else
        {
                // reboot the hardware.
                sys_reboot(SYS_REBOOT_COLD);
                return SMF_EVENT_HANDLED;
        }
}