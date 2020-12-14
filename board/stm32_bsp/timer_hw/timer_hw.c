#include "timer_hw.h"
#include "stdlib.h"
#include "interrupt_hw.h"

#define TIMER_TIM_DEV					TIM2
#define TIM_PRESCALER					23
#define TIM_PERIOD					2000
#define TIMER_TIM_DEV_CLK				RCC_APB1Periph_TIM2
#define TIMER_TIM_INTERRUPT_PRIORITY                    TIMER_SERVICE_INTERRUPT_PRIORITY
#define TIMER_UPDATE_HANDLE				TIM2_IRQHandler

#define MAX_TIMER   8

static void timer_hardware_clock_init(void);
static void timer_hardware_timer_init(void);

static struct Timer_Hw_t timer_hw[MAX_TIMER];

void timer_hw_init(void){
	timer_hardware_clock_init();
	timer_hardware_timer_init();
        for(int i=0;i<MAX_TIMER;i++){
                timer_hw[i].ticker=0;
                timer_hw[i].timeout=0;
                timer_hw[i].is_active=0;
        }
}

struct Timer_Hw_t* timer_hw_create(void){
	/* get timer_hw object from object pool  */
	for(int i=0;i<MAX_TIMER;i++){
		if(timer_hw[i].is_active==0){
			return &timer_hw[i];
		}
	}
	return NULL;
}

static void timer_hardware_clock_init(void) {
	RCC_APB1PeriphClockCmd(TIMER_TIM_DEV_CLK,ENABLE);
}

static void timer_hardware_timer_init(void){

	TIM_TimeBaseInitTypeDef TimBase_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TimBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimBase_InitStructure.TIM_Prescaler  = TIM_PRESCALER;
	TimBase_InitStructure.TIM_Period = TIM_PERIOD;
	TimBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimBase_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMER_TIM_DEV,&TimBase_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = TIMER_TIM_INTERRUPT_PRIORITY;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIMER_TIM_DEV,TIM_FLAG_Update);
	TIM_ITConfig(TIMER_TIM_DEV,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIMER_TIM_DEV,ENABLE);
}

void TIMER_UPDATE_HANDLE(void) {

	if(TIM_GetITStatus(TIMER_TIM_DEV, TIM_IT_Update) != RESET) {
                for(int i=0;i<MAX_TIMER;i++){
                        if(timer_hw[i].is_active){
                                timer_hw[i].ticker++;
                                if(timer_hw[i].ticker>=timer_hw[i].timeout){
                                        timer_hw[i].ticker=timer_hw[i].timeout;
                                }
                        }
                }
		TIM_ClearFlag(TIMER_TIM_DEV, TIM_FLAG_Update);
	}
}

