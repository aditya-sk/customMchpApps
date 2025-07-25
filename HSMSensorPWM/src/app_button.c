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

LOG_MODULE_REGISTER(button_LOG);

// get devicetree node
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
// callback structures
struct gpio_callback buttonCallback;
// to track the last button press
static uint32_t lastTime = 0U;

volatile enum bno055_sensor_channel currentChannel = BNO055_SENSOR_CHAN_CALIBRATION_SGAM;


// TODO #4: Parent state that handles the events propogated from child.
enum smf_state_result ChannelSelect_RUN(void* ptrData)
{
        
        
}


int init_SW0(void)
{
    int ret;
        if (!gpio_is_ready_dt(&button)) 
        {
                LOG_ERR("Error: button device %s is not ready\n", button.port->name);
                return -1;
        }

        ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
        if (ret != 0) 
        {
                LOG_ERR("Error %d: failed to configure %s pin %d\n", ret, button.port->name, button.pin);
                return -1;
        }

        ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
        if (ret != 0) 
        {
                LOG_ERR("Error %d: failed to configure interrupt on %s pin %d\n", ret, button.port->name, button.pin);
                return -1;
        }

        gpio_init_callback(&buttonCallback, buttonPressed_handler, BIT(button.pin));
        gpio_add_callback(button.port, &buttonCallback);
        return 0;
}


void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        
        uint32_t currentTime = k_uptime_get_32();
        if(currentTime - lastTime > DEBOUNCE_DELAY_MS)
        {
                LOG_INF("Captured the Button event\r\n");
                /* Generate Button Press Event */
                k_event_post(&stateMachineInfo.smf_event, EVENT_BTN_PRESS);
                lastTime = currentTime; 
        }else{
                LOG_INF("Debounced button press ignored\r\n");
        }
}


