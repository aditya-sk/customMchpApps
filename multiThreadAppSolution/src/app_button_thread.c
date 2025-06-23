// Zephyr library header files
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>

// Application header with structure and function prototype
#include <app_pwm_thread.h>
#include <app_button_thread.h>
#include <app_sensor_thread.h>

// bno055 library header file
#include <bno055.h>

// get devicetree node
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
// callback structures
struct gpio_callback buttonCallback;
// to track the last button press
static uint32_t lastTime = 0U;


LOG_MODULE_REGISTER(button_thread_log, LOG_LEVEL_INF);

// create binary semaphore with inital value as zero
K_SEM_DEFINE(sw_sem, 0, 1);

volatile enum bno055_sensor_channel currentChannel = BNO055_SENSOR_CHAN_CALIBRATION_SGAM;

// check for the swo readiness, configuration, and interrupt callback setup
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

// interrupt handler for sw0
void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        /* Generate Button Press Event */
        uint32_t currentTime = k_uptime_get_32();
        if(currentTime - lastTime > DEBOUNCE_DELAY_MS)
        {
                LOG_INF("Captured the Button event\r\n");
                k_sem_give(&sw_sem);
                lastTime = currentTime; 
        }else{
                LOG_INF("Debounced button press ignored\r\n");
        }
}




// TODO #4: thread to switch between channels on binary semaphore take

void channelSelect_Thread(void* arg1, void* arg2, void* arg3)
{
        while(1){
        if(k_sem_take(&sw_sem, K_FOREVER) == 0){

                switch(currentChannel){

                        case BNO055_SENSOR_CHAN_CALIBRATION_SGAM:
                                currentChannel = BNO055_SENSOR_CHAN_LINEAR_ACCEL_XYZ;
                                break;

                        case BNO055_SENSOR_CHAN_LINEAR_ACCEL_XYZ:
                                currentChannel = BNO055_SENSOR_CHAN_GRAVITY_XYZ;
                                break;

                        case BNO055_SENSOR_CHAN_GRAVITY_XYZ:
                                currentChannel = BNO055_SENSOR_CHAN_EULER_YRP;
                                break;

                        case BNO055_SENSOR_CHAN_EULER_YRP:
                                currentChannel = BNO055_SENSOR_CHAN_QUATERNION_WXYZ;
                                break;
    
                        case BNO055_SENSOR_CHAN_QUATERNION_WXYZ:
                                currentChannel = BNO055_SENSOR_CHAN_LINEAR_ACCEL_XYZ;
                                break;

                        default:
                                currentChannel = BNO055_SENSOR_CHAN_CALIBRATION_SGAM;
                                break;

                        }
                }
        }
}