// Zephyr library header files
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

// Application header with Enums, Structure and function prototype
#include <app_pwm_state.h>

// TODO #2: get the devicetree node


// Global variables for calculation max, periond and direction to multiple or divide by 2
static uint32_t max_period;
static uint32_t period;
uint8_t direction = 0U;


/*
 * FSM 5 State Transition:
 *
 *	    ---> Init_ENTRY --> Init_RUN --> Init_EXIT -----------------------------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> caliberatePWM_ENTRY --> caliberation_EXIT -------------------------|
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> ledPWM_RUN --> firstState_EXIT  -----------------------------------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |----> checkPeriod_RUN --> stateSecond_exit ----------------------------|
 *                                                                              |
 *      |-----------------------------------------------------------------------|
 *      |
 *      |--> errorHandle --> FIRST_S1    ---------------------------------------|
 *      |                                                                       |
 *                       --> INITIAL_S0  ---------------------------------------|
 */    


// TODO #3: Checking if the PWM is ready or not 
void initPWM_ENTRY(void* ptrData) 

//TODO #4: Set the parameter to zero
void initPWM_RUN(void* ptrData)

//TODO #5: Exit from init PWM state
void initPWM_EXIT(void* ptrData)

// TODO #6: Calibrating for Maximumm period based on MCU clock
void calibratePWM_ENTRY(void* ptrData)

// Exiting from calibration state
void calibration_EXIT(void* ptrData)
{
    printk("Completed the caliberation for MAX period\r\n");
}

// TODO #7 Set Period and pulse for PWM using PWM interface API
void ledPWM_RUN(void* ptrData)

// Exiting from first state with a PWM Period
void firstState_EXIT(void* ptrData)
{
    printk("Exit S1 State\r\n");
}

// Reduce the period by factor of 2
void secondState_ENTRY(void* ptrData) 
{
    printk("Enter S2 State\r\n");
}

// TODO #8: Reduce the period by factor of 2
void checkPeriod_RUN(void* ptrData)

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
