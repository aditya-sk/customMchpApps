#ifndef APP_PWM_THREAD_H
#define APP_PWM_THREAD_H


// Assuming for 7-bit 
#define MIN_PERIOD PWM_SEC(1U) / 128U
#define MAX_PERIOD (PWM_SEC(1U) / 2U)



#define TIMEOUT_TWO_SEC K_SECONDS(2U)
#define TIMEOUT_SIX_SEC K_SECONDS(6U)

#define PERIOD_HALF_DIVISOR 2U


/* Object used in multiple files*/
extern struct pwm_dt_spec pwmBlink;

void initSYSTEM();
void ledPWM_Thread(void*, void*, void*);



#endif // APP_PWM_THREAD_H