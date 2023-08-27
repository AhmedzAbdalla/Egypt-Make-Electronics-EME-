#include "Inc/LIB/DELAY.h"
#include "Inc/MCAL/SCB/SCB_interface.h"
#include "Inc/MCAL/GPIO/GPIO_interface.h"
#include "Inc/MCAL/SYSTICK/SYSTICK_Interface.h"
#include "Inc/MCAL/GPT/GPT_interface.h"

#include "HAL/LCD/LCD_Interface.h"
#include "HAL/keypad/keypad_interface.h"

#include "hw_gpio.h"

#define SET_Alternative_Function *((volatile uint32*)0xE000E100) |= (1 << (19));

extern const SCB_ConfigType SCBConfig[SCB_CLOCK_SETTINGS_SIZE];
extern const Port_ConfigType portConfig[NUM_PINS];

u8 ret ;

extern const GPT_ConfigType gptConfig[size];

volatile u8 counter_sec;
volatile u8 counter_min;
volatile u8 current_state = 0;

static void App_delayMs(u32 Copy_u32Delay)
{
    u32 Local_u32Counter = 0;
    for(Local_u32Counter = 0; Local_u32Counter < Copy_u32Delay*700; Local_u32Counter++)
    {
        asm("NOP");
    }
}

void test(void);

s32 on_time = 50000 - 100;

void main(void)
{

    SCB_Init(SCBConfig);
    SCB_InitClock(CLOCK_0);
    DIO_Config(portConfig);
    GPT_Init(gptConfig);

    GPIO_PORTF_LOCK_R = 0X4C4F434B;

    GPIO_PORTF_CR_R = 0X1F;
    GPIO_PORTF_DEN_R = 0X1F;
    GPIO_PORTF_AFSEL_R = 0x04;
    GPIO_PORTF_PCTL_R = 0x700;

    SET_Alternative_Function


    while (1)
    {


		/*from high intensity to low intensity*/
        for(on_time =50000-100; on_time > 1; )
        {
            on_time-=1000;
            GPT_Init_PWM(GPT_CHNL_TIMER1  , on_time);
            App_delayMs(100);

        }
        App_delayMs(1000);
		/*from low intensity to high intensity*/
        for(on_time =100; on_time < 49999; )
        {
            on_time+=1000;
            GPT_Init_PWM(GPT_CHNL_TIMER1  , on_time);
            App_delayMs(100);

        }

    }
}

void test(void)
{

        TOGGLE_BIT(GPIO_PORTF_DATA_R , 2);
}

