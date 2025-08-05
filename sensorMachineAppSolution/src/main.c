#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/smf.h>

// sensor library header file
#include <bno055.h>

//application header file
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>



/* User defined object */
struct FSMContext stateMachineInfo;



// TODO #1: Event Based State Machine with funciton pointer mapping with respective states. 
const struct smf_state stateTable[NUM_STATES] = 
{
    [PWM_INITIAL] = SMF_CREATE_STATE(initPWM_GPIO_ENTRY, initPWM_RUN, initPWM_EXIT, NULL, NULL),
    [CALIBRATION] = SMF_CREATE_STATE(calibratePWM_ENTRY, NULL, calibration_EXIT, NULL, NULL),
    [FIRST_STATE] = SMF_CREATE_STATE(NULL, ledPWM_RUN, NULL, NULL, NULL),
    [SECOND_STATE] = SMF_CREATE_STATE(NULL, checkPeriod_RUN, NULL, NULL, NULL),
    [SENSOR_INIT] = SMF_CREATE_STATE(initI2CNode_ENTRY, NULL, NULL, NULL, NULL),
    [SENSOR_SAMPLE] = SMF_CREATE_STATE(NULL, dataFetch_RUN, NULL, NULL, NULL),
    [SENSOR_CONSOLE] = SMF_CREATE_STATE(NULL, dataFormat_RUN, NULL, NULL, NULL),
    [ERROR_STATE] = SMF_CREATE_STATE(NULL, errorHandle_RUN, NULL, NULL, NULL)
    
};


// TODO #10: Main function to initialize the SMF, Events, and start the state machine with a specific state.
int main(void)
{
        int ret;
        /* Set initial state for FSM*/
        smf_set_initial(SMF_CTX(&stateMachineInfo), &stateTable[PWM_INITIAL]);
        /* Initialize the event */
        k_event_init(&stateMachineInfo.smf_event);
            while(true)
            {   
                /* unBlocking until an event is detected */
                stateMachineInfo.events = k_event_wait(&stateMachineInfo.smf_event, EVENT_BTN_PRESS, false, K_NO_WAIT);
                // start SMF for PWMMachine
                ret = smf_run_state(SMF_CTX(&stateMachineInfo));
                if(ret < 0)
                {
                    return -1;
                }
            }
        return 0;
}

