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
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
// callback structures
struct gpio_callback buttonCallback;
// to track the last button press
static uint32_t lastTime = 0U;

// TODO #8: interrupt service routine for GPIO
void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        /* Generate Button Press Event */
        uint32_t currentTime = k_uptime_get_32();
        if(currentTime - lastTime > DEBOUNCE_DELAY_MS)
        {
                printk("Captured the Button event\r\n");
                k_event_post(&stateMachineInfo.smf_event, EVENT_BTN_PRESS);
                lastTime = currentTime; 
        }else{
                printk("Debounced button press ignored\r\n");
        }
}

// TODO #9: switch between two machine on interrupt event
void FSM_select(struct FSMContext* infoData)
{
        if(infoData->ctx.current == &stateTable[FIRST_STATE] || infoData->ctx.current == &stateTable[SECOND_STATE])
        {
                pwm_set_dt(&pwmBlink, 0, 0);
                smf_set_state(SMF_CTX(infoData), &stateTable[SENSOR_INIT]);
        }else{
                pwm_set_dt(&pwmBlink, 0, 0);
                smf_set_state(SMF_CTX(infoData), &stateTable[FIRST_STATE]);
        }
        infoData->events &= ~EVENT_BTN_PRESS;  // Clear event *after* handling
        k_event_clear(&infoData->smf_event, EVENT_BTN_PRESS);
}



// TODO #7: GPIO with interrupt configuration
int init_SW0(void)
{
    int ret;
        if (!gpio_is_ready_dt(&button)) 
        {
                printk("Error: button device %s is not ready\n", button.port->name);
                //smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_SERR]);
                return -1;
        }

        ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
        if (ret != 0) 
        {
                printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name, button.pin);
                //smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_SERR]);
                return -1;
        }

        ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
        if (ret != 0) 
        {
                printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, button.port->name, button.pin);
                //smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_SERR]);
                return -1;
        }

        gpio_init_callback(&buttonCallback, buttonPressed_handler, BIT(button.pin));
        gpio_add_callback(button.port, &buttonCallback);
        return 0;
}


