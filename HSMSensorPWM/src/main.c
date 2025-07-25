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

};


// TODO #5: Main function to initialize the SMF, Events, and start the state machine with a specific state.
int main(void)
{

        return 0;
}