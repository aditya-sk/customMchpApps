#ifndef APP_BUTTON_STATE_H
#define APP_BUTTON_STATE_H

#include <app_pwm_state.h>

// List of Events
#define EVENT_BTN_PRESS BIT(0) 



#define DEBOUNCE_DELAY_MS 1000

#define SW0_NODE        DT_ALIAS(userbutton)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

extern volatile enum bno055_sensor_channel currentChannel;


enum smf_state_result ChannelSelect_RUN(void* ptrData);

int init_SW0(void);
void buttonPressed_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins);


#endif // APP_BUTTON_STATES_H