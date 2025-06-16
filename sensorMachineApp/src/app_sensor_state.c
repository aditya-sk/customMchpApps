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



// TODO #3: Check if the I2C slave device is ready or not
void initI2CNode_ENTRY(void* ptrData)
{


	
}

// TODO #4: fetch the data from sensor, filter based on the channel of the sensor, and transition to SENSOR_CONSOLE
void dataFetch_RUN(void* ptrData)
{


	 
}

// TODO #5: format the buffered data to present on serial terminal and transition to SENSOR_SAMPLE state
void dataFormat_RUN(void* ptrData)
{


	 
}

