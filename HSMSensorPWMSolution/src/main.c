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



// Application header with Enums, Structure and function prototype
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>

// sensor library header file
#include <bno055.h>

LOG_MODULE_REGISTER(main_LOG);

/* User defined object */
struct FSMContext stateMachineInfo;


// TODO #1: Event Based HSM with funciton pointer mapping with states. 
const struct smf_state stateTable[NUM_STATES] = 
{
    [PARENT] = SMF_CREATE_STATE(NULL, ChannelSelect_RUN, NULL, NULL, NULL),
    [INITIALIZE] = SMF_CREATE_STATE(init_ENTRY, init_RUN, init_EXIT, &stateTable[PARENT], NULL),
    [LED_BLINK] = SMF_CREATE_STATE(NULL, ledPWM_RUN, ledPWM_EXIT, &stateTable[PARENT], NULL),
    [SAMPLE_FORMAT] = SMF_CREATE_STATE(NULL, dataFetch_RUN, NULL, &stateTable[PARENT], NULL),
    [ERROR_STATE] = SMF_CREATE_STATE(NULL, errorHandle_RUN, NULL, NULL, NULL)
    
};


// TODO #5:Main function to initialize the SMF, Events, and start the state machine with a specific state.
int main(void)
{
        int ret;
        /* Set initial state for FSM*/
        smf_set_initial(SMF_CTX(&stateMachineInfo), &stateTable[INITIALIZE]);
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
                k_sleep(TIMEOUT_FOUR_SEC);
            }
        return 0;
}

