#ifndef APP_BUTTON_THREAD_H
#define APP_BUTTON_THREAD_H

#include <app_pwm_thread.h>


#define EVENT_BTN_PRESS BIT(0) // bit 0 for sw0 flag
#define DEBOUNCE_DELAY_MS 1000

#define SW0_NODE        DT_ALIAS(userbutton)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif


extern volatile enum bno055_sensor_channel currentChannel;

int init_SW0(void);
void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void channelSelect_Thread(void*, void*, void*);




#endif // APP_BUTTON_THREAD_H