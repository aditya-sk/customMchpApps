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


LOG_MODULE_REGISTER(sensor_thread_log, LOG_LEVEL_INF);

const struct device* bno055_node = DEVICE_DT_GET(DT_NODELABEL(bno055));


// initialize i2c controller with sensor node
void initI2CNode()
{

    if (!device_is_ready(bno055_node)) {
		LOG_ERR("Device %s is not ready\r\n", bno055_node->name);
	}
	struct sensor_value config = {
		.val1 = BNO055_MODE_NDOF,
		.val2 = 0,
	};
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, SENSOR_ATTR_CONFIGURATION, &config);
	config.val1 = BNO055_POWER_NORMAL;
	config.val2 = 0;
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, BNO055_SENSOR_ATTR_POWER_MODE, &config);
	
}



// TODO #6: thread fetches the sample, reads a specific channel and store the data into respective buffer

void fetchGet_Thread(void* arg1, void* arg2, void* arg3)
{
		
}

