#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>
// sensor library header file
#include <bno055.h>

// Application header with Enums, Structure and function prototype
#include <app_pwm_state.h>
#include <app_button.h>
#include <app_sensor_state.h>


// TODO #2: get the devicetree node and allocate memory for data
const struct device* bno055_node = DEVICE_DT_GET(DT_NODELABEL(bno055));

static struct sensor_value accelerometerData[3];

// TODO #3: Check if the I2C slave device is ready or not
void initI2CNode_ENTRY(void* ptrData)
{

    if (!device_is_ready(bno055_node)) {
		printk("Device %s is not ready\r\n", bno055_node->name);
		smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
	}
	struct sensor_value config = {
		.val1 = BNO055_MODE_ACC_MAG_GYRO,
		.val2 = 0,
	};
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, SENSOR_ATTR_CONFIGURATION, &config);
	config.val1 = BNO055_POWER_NORMAL;
	config.val2 = 0;
	sensor_attr_set(bno055_node, SENSOR_CHAN_ALL, BNO055_SENSOR_ATTR_POWER_MODE, &config);
	smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[SENSOR_SAMPLE]);
	
}

// TODO #4: fetch the data from sensor, filter based on the channel of the sensor, and transition to SENSOR_CONSOLE
void dataFetch_RUN(void* ptrData)
{

	int ret;
	struct FSMContext* info = (struct FSMContext*)ptrData;
    if (info->events & EVENT_BTN_PRESS)
    {
        FSM_select(info);
	}else{
		ret = sensor_sample_fetch(bno055_node);
		if(ret < 0){
			smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[ERROR_STATE]);
		}
		
		// linear acceleration and gravity
		sensor_channel_get(bno055_node, SENSOR_CHAN_ACCEL_XYZ, accelerometerData);
		smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[SENSOR_CONSOLE]); 
	}
	 
}

// TODO #5: format the buffered data to present on serial terminal and transition to SENSOR_SAMPLE state
void dataFormat_RUN(void* ptrData)
{

	struct FSMContext* info = (struct FSMContext*)ptrData;
    if (info->events & EVENT_BTN_PRESS)
    {
        FSM_select(info);
	}else{

	printk("ACCEL: X(m.s-2)[%d.%06d] Y(m.s-2)[%d.%06d] Z(m.s-2)[%d.%06d]\n", accelerometerData[0].val1, accelerometerData[0].val2, accelerometerData[1].val1, accelerometerData[1].val2, accelerometerData[2].val1, accelerometerData[2].val2);
	//k_sleep(K_MSEC(500));
	smf_set_state(SMF_CTX(&stateMachineInfo), &stateTable[SENSOR_SAMPLE]); 
	}
	 
}

