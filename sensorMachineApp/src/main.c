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
 
    
};


// TODO #10: Main function to initialize the SMF, kernel events, and start the state machine with a specific state.
int main(void)
{
        

}

