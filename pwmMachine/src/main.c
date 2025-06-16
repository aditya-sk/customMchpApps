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

// TODO #1: Finite State Machine table to redirect the function pointer with the respective states.
const struct smf_state stateTable[NUM_STATES];

// TODO #9:Main function to initialize the SMF and start the state machine
int main(void)
{

}

