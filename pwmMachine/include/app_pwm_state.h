#ifndef APP_PWM_STATE_H
#define APP_PWM_STATE_H



// Assuming for 7-bit 
#define MIN_PERIOD PWM_SEC(1U) / 128U    // 7,812,500 ns
#define MAX_PERIOD PWM_SEC(1U)          // // 1 sec = 1,000,000,000 ns


#define TIMEOUT_FOUR_SEC K_SECONDS(4U)
#define TIMEOUT_TWO_SEC K_SECONDS(2U)

#define PERIOD_DOUBLE 2U
#define PERIOD_HALF_DIVISOR 2U

#define TWO_FACTOR
typedef enum  {
    PWM_INITIAL,     // LED OFF
    CALIBRATION,
    FIRST_STATE,       // Slow Blink
    SECOND_STATE,      // Fast Blink
    ERROR_STATE,     // Error state
    NUM_STATES      // for array sizing (Just optional)
}APP_State;

/* User defined object */
struct FSMContext 
{   /* This must be first */
    struct smf_ctx ctx;
    // /* Events */
    // struct k_event smf_event;
    // uint32_t events;
};
/* Object used in multiple files*/
extern struct pwm_dt_spec pwmBlink;
extern struct FSMContext stateMachineInfo;
extern const struct smf_state stateTable[NUM_STATES];

// Init sate function prototype
void initPWM_ENTRY(void* ptrData);
void initPWM_RUN(void* ptrData);
void initPWM_EXIT(void* ptrData);

// Caliberation state function prototype
void calibratePWM_ENTRY(void* ptrData);
void calibration_EXIT(void* ptrData);

// S1 funciton prototype
void ledPWM_RUN(void* ptrData);
void firstState_EXIT(void* ptrData);

//S2 function prototype
void secondState_ENTRY(void* ptrData);
void checkPeriod_RUN(void* ptrData);


// state with only error handling and resume
void errorHandle_RUN(void* ptrData);


#endif // APP_PWM_STATES_H