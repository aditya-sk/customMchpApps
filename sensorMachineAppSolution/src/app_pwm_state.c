
// zephyr library header files
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

// application header with Enums, Structure and function prototype
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>

// TODO #1: get the devicetree node
struct pwm_dt_spec pwmBlink  = PWM_DT_SPEC_GET(DT_ALIAS(userled));


// Global variables
static uint32_t max_period;
static uint32_t period;
uint8_t direction = 0U;


/*
 * FSM 4 State Transition:
 *
 *	    ---> Init_ENTRY --> Init_RUN --> Init_EXIT -----------------------------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> stateOne_ENTRY --> stateOne_RUN --> stateOne_exit -----------------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> stateSecond_ENTRY --> stateSecond_RUN --> stateSecond_exit --------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> errorHandle --> FIRST_S1    ---------------------------------------|
 *      |                                                                       |
 *                       --> INITIAL_S0  ---------------------------------------|
 */    


// TODO #: Checking if the PWM is ready or not 
void initPWM_GPIO_ENTRY(void* ptrData) 
{
        printk("Enter to Init (LED OFF)\r\n");
        
        if(!pwm_is_ready_dt(&pwmBlink))
        {
            printk("Error occure while PWM init\r\n");
            // Transition to error state
            smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
        }
        if(0 != init_SW0())
        {
            printk("Error occure while GPIO init\r\n");
            // Transition to error state
            smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
        }
        k_sleep(TIMEOUT_FOUR_SEC); 
}


//TODO #: Set the parameter to zero
void initPWM_RUN(void* ptrData)
{

        pwm_set_dt(&pwmBlink, 0, 0);  //(0 duty cycle)
        printk("Completed and PWM is ready\r\n");
        smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[CALIBRATION]); // Transition to caliberation mode
        k_sleep(TIMEOUT_FOUR_SEC); 

}

//TODO #: Exit from init PWM state
void initPWM_EXIT(void* ptrData)
{
        printk("Completed the Init PWM LED\r\n");
}

// TODO #: Calibrating for Maximumm period based on MCU clock
void calibratePWM_ENTRY(void* ptrData)
{
        printk("In CALIBRATION state\n");
        // Calibrate maximum period
        max_period = MAX_PERIOD;
        printk("Calibrated for max period %u nsec\r\n", max_period);
        k_sleep(TIMEOUT_FOUR_SEC); 
        while (pwm_set_dt(&pwmBlink, max_period, max_period / PERIOD_HALF_DIVISOR)) 
        {
            max_period /= PERIOD_HALF_DIVISOR;
            if (max_period < (5U * MIN_PERIOD)) 
            {
                printk("Error: PWM device doesn't support the period at least %lu\n", 4U * MIN_PERIOD);
                // Transition to error State
                smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
                break;  
            }
        }
        
        period = max_period;
        k_sleep(TIMEOUT_FOUR_SEC); 
        smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[FIRST_STATE]); 

}

// Exiting from calibration state
void calibration_EXIT(void* ptrData)
{
        printk("Completed the caliberation for MAX period\r\n");
}


// Todo # Set Period and pulse for PWM using PWM interface API
// Note: need to add the button pressed event and state transition
void ledPWM_RUN(void* ptrData)
{
    int ret = 0;
        struct FSMContext* info = (struct FSMContext*)ptrData;
        if (info->events & EVENT_BTN_PRESS)
        {
            FSM_select(info);
        }else{
            printk("Period %u nsec\n", period);
            ret = pwm_set_dt(&pwmBlink, period, period / PERIOD_HALF_DIVISOR);
            if(ret)
            {
                printk("Error %d: failed to set pulse width in state S1\n", ret);
                smf_set_state(SMF_CTX(info), &stateTable[ERROR_STATE]); // Transition to S0 in case of error
            }

        k_sleep(TIMEOUT_FOUR_SEC); // Stay in S1 for 4 seconds before transitioning
        smf_set_state(SMF_CTX(info), &stateTable[SECOND_STATE]); // Transition to S2 (fast blink)
        }
    
}

// S2: Fast Blink with PWM LED
//Note: need to work on event flag for button interrupt and change state trnasistion from here two situation
void checkPeriod_RUN(void* ptrData) 
{
    struct FSMContext* info = (struct FSMContext*)ptrData;
        if (info->events & EVENT_BTN_PRESS)
        {
            FSM_select(info);
        }else{
            period = direction ? (period * PERIOD_DOUBLE) : (period / PERIOD_HALF_DIVISOR);
            printk("S2 %s\r\n", direction ? "Increment period" : "Decrement Period");
            if (period > max_period) {
                period = max_period / PERIOD_HALF_DIVISOR;
                direction = 0U;
            }else if (period < MIN_PERIOD){
                period = MIN_PERIOD * PERIOD_DOUBLE;
                direction = 1U;
            }
        
        k_sleep(TIMEOUT_FOUR_SEC); // Stay in S2 for 4 seconds before transitioning
        smf_set_state(SMF_CTX(info), &stateTable[FIRST_STATE]); // Transition back to S1 (slow blink)
        }
    
}


// Error State has only error actions
void errorHandle_RUN(void* ptrData)
{
    const struct FSMContext* info = (const struct FSMContext*)ptrData;
        if(info->ctx.previous == &stateTable[FIRST_STATE] || info->ctx.previous == &stateTable[SECOND_STATE])
        {
                smf_set_state(SMF_CTX(&info), &stateTable[CALIBRATION]);
        }else{
                // need to work on resetting the PMW module and its parameter
                smf_set_state(SMF_CTX(&info), &stateTable[PWM_INITIAL]);
        }

}


