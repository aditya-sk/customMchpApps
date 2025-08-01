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


// TODO #2: get the devicetree node and allocate memory for data
const struct device* bno055_node = DEVICE_DT_GET(DT_NODELABEL(bno055));
static struct sensor_value linearAcceleration[3]; 
static struct sensor_value calib[4];
static struct sensor_value gravityData[3];
static struct sensor_value eulerData[3];
static struct sensor_value quaternionData[3];


// TODO #3: fetch the data from sensor, filter based on the channel of the sensor, and transition to SENSOR_CONSOLE
enum smf_state_result dataFetch_RUN(void* ptrData)
{
	struct FSMContext* info = (struct FSMContext*)ptrData;
	int ret;
    if (info->events & EVENT_BTN_PRESS)
    {
		return SMF_EVENT_PROPAGATE;
	}else{
		ret = sensor_sample_fetch(bno055_node);
		if(ret < 0){
			LOG_ERR("Error accurred while sensor data fetching\r\n");
		}
		switch(currentChannel){
				
			case BNO055_SENSOR_CHAN_CALIBRATION_SGAM:
				
				sensor_channel_get(bno055_node, BNO055_SENSOR_CHAN_CALIBRATION_SGAM, calib);
				LOG_INF("CALIB: SYS[%d] GYR[%d] ACC[%d] MAG[%d]\n", calib[0].val1, calib[1].val1, calib[2].val1, calib[3].val1);
				break;

			case BNO055_SENSOR_CHAN_LINEAR_ACCEL_XYZ:
				sensor_channel_get(bno055_node, BNO055_SENSOR_CHAN_LINEAR_ACCEL_XYZ, linearAcceleration);
				LOG_INF("ACCEL [m.s-2] -> X: [%d.%06d] Y: [%d.%06d] Z: [%d.%06d]\n", 
					linearAcceleration[0].val1, linearAcceleration[0].val2, linearAcceleration[1].val1, linearAcceleration[1].val2, linearAcceleration[2].val1, linearAcceleration[2].val2);
				
				break;

			case BNO055_SENSOR_CHAN_GRAVITY_XYZ:
				
				sensor_channel_get(bno055_node, BNO055_SENSOR_CHAN_GRAVITY_XYZ, gravityData);
				LOG_INF("GRAVITY [m.s-2] -> X: [%d.%06d] Y: [%d.%06d] Z: [%d.%06d]\n",
			       gravityData[0].val1, gravityData[0].val2, gravityData[1].val1, gravityData[1].val2, gravityData[2].val1,gravityData[2].val2);

				break;

			case BNO055_SENSOR_CHAN_EULER_YRP:
				
				sensor_channel_get(bno055_node, BNO055_SENSOR_CHAN_EULER_YRP, eulerData);
				LOG_INF("EULER [rad.s-1]: X->Roll [%d.%06d] Y->Pitch [%d.%06d] Z-Yaw [%d.%06d]\n",
			       eulerData[0].val1, eulerData[0].val2, eulerData[1].val1, eulerData[1].val2, eulerData[2].val1, eulerData[2].val2);

				break;

			case BNO055_SENSOR_CHAN_QUATERNION_WXYZ:
				
				sensor_channel_get(bno055_node, BNO055_SENSOR_CHAN_QUATERNION_WXYZ, quaternionData);
				LOG_INF("QUATERNION: W [%d.%06d] X [%d.%06d] Y [%d.%06d] Z [%d.%06d]\n",
			       quaternionData[0].val1, quaternionData[0].val2, quaternionData[1].val1, quaternionData[1].val2, quaternionData[2].val1, quaternionData[2].val2, quaternionData[2].val1, quaternionData[2].val2);

				break;
			
			default:
				sensor_channel_get(bno055_node, currentChannel, linearAcceleration);
				LOG_INF("ACCEL [m.s-2] -> X [%d.%06d] Y [%d.%06d] Z [%d.%06d]\n", 
					linearAcceleration[0].val1, linearAcceleration[0].val2, linearAcceleration[1].val1, linearAcceleration[1].val2, linearAcceleration[2].val1, linearAcceleration[2].val2);

				break;
		}

	}
	smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[SAMPLE_FORMAT]); 
	return SMF_EVENT_HANDLED;
	 
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

