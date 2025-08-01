#ifndef APP_PWM_STATE_H
#define APP_PWM_STATE_H



#define PERIOD_NS PWM_SEC(1U)   // 1 sec = 1,000,000,000 ns

#define TIMEOUT_FOUR_SEC K_SECONDS(4U)
#define TIMEOUT_TWO_SEC K_SECONDS(2U)

#define PERIOD_DIVISOR 3U

typedef enum  {
    PARENT,       // child to parent
    INITIALIZE,
    LED_BLINK,          // period
    SAMPLE_FORMAT,      // fetch the sample and console 
    ERROR_STATE,        // Error state
    NUM_STATES          // array size (Just optional)
}APP_State;

/* User defined object */
struct FSMContext 
{   /* This must be first */
    struct smf_ctx ctx;
    /* Events for toggling */
    struct k_event smf_event;
    uint32_t events;
};
/* Object used in multiple files*/
extern struct pwm_dt_spec pwmBlink;
extern struct FSMContext stateMachineInfo;
extern const struct smf_state stateTable[NUM_STATES];


// Init sate function prototype
void init_ENTRY(void* ptrData);
enum smf_state_result init_RUN(void* ptrData);
void init_EXIT(void* ptrData);



// S1 funciton prototype
enum smf_state_result ledPWM_RUN(void* ptrData);
void ledPWM_EXIT(void* ptrData);

// state with only error handling and resume
enum smf_state_result errorHandle_RUN(void* ptrData);


#endif // APP_PWM_STATES_H