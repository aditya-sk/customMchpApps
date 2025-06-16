// Zephyr header
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

// application header
#include <app_pwm_state.h>


/* User defined object */
struct FSMContext stateMachineInfo;

// TODO #: Finite State Machine table to redirect the function pointer with the respective states.
const struct smf_state stateTable[NUM_STATES] = 
{
    [PWM_INITIAL] = SMF_CREATE_STATE(initPWM_ENTRY, initPWM_RUN, initPWM_EXIT, NULL, NULL),
    [CALIBRATION] = SMF_CREATE_STATE(calibratePWM_ENTRY, NULL, calibration_EXIT, NULL, NULL),
    [FIRST_STATE] = SMF_CREATE_STATE(NULL, ledPWM_RUN, firstState_EXIT, NULL, NULL),
    [SECOND_STATE] = SMF_CREATE_STATE(secondState_ENTRY, checkPeriod_RUN, NULL, NULL, NULL),
    [ERROR_STATE] = SMF_CREATE_STATE(NULL, errorHandle_RUN, NULL, NULL, NULL)
    
};

// TODO #:Main function to initialize the SMF and start the state machine
int main(void)
{
        int ret;
        /* Set initial state for FSM*/
        smf_set_initial(SMF_CTX(&stateMachineInfo), &stateTable[PWM_INITIAL]);

        while(true)
        {   // start SMF for PWMMachine
            ret = smf_run_state(SMF_CTX(&stateMachineInfo));
            if(ret < 0)
            {
                return -1;
            }
            k_sleep(TIMEOUT_FOUR_SEC);
        }
        return 0;
}

