// Zephyr library header files
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

// Application header with Enums, Structure and function prototype
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>


// TODO #6: get devicetree node


// TODO #8: interrupt service routine for GPIO
void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{


}


// TODO #9: switch between two machine on interrupt event
void FSM_select(struct FSMContext* infoData)
{



}


// TODO #7: GPIO with interrupt configuration
int init_SW0(void)
{


}


