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



LOG_MODULE_REGISTER(sensor_LOG);


// get the devicetree node and allocate memory for data
const struct device* bno055_node = DEVICE_DT_GET(DT_NODELABEL(bno055));

static struct sensor_value linearAcceleration[3]; 
static struct sensor_value calib[4];
static struct sensor_value gravityData[3];
static struct sensor_value eulerData[3];
static struct sensor_value quaternionData[4];

// TODO #3: fetch the data from sensor, filter based on the channel of the sensor, and transition to SENSOR_CONSOLE
enum smf_state_result dataFetch_RUN(void* ptrData)
{
	 
}


int initI2CNode()
{

    if (!device_is_ready(bno055_node)) {
		LOG_ERR("Device %s is not ready\r\n", bno055_node->name);
		smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
		return -1;
	}
	struct sensor_value config = {
		.val1 = BNO055_MODE_NDOF,
		.val2 = 0,
	};
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, SENSOR_ATTR_CONFIGURATION, &config);
	config.val1 = BNO055_POWER_NORMAL;
	config.val2 = 0;
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, BNO055_SENSOR_ATTR_POWER_MODE, &config);
	return 0;
	
}

