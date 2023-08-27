/****************************
 * GPT.c
 *
 *  Created on: 25/8/2024
 *      Author: HOSSENI
 ****************************/
#include "../../../Inc/MCAL/GPT/GPT_config.h"
#include "../../../Inc/MCAL/GPT/GPT_interface.h"
#include "../../../Inc/MCAL/GPT/GPT_private.h"

#include "../../../LIB/STD_TYPES.h"
#include "../../../LIB/BIT_MATH.h"

/* Define Callback Global Variable */
static void (*GPT_Timer0_CallBack) (void);

static uint32 GPT_GetTimerBaseAdr(GPT_ChannelType gptChannelid)
{
    uint32 baseAdr = GPT_0_BASE;
    switch(gptChannelid)
    {
    case GPT_CHNL_TIMER0:
        baseAdr = GPT_0_BASE;
        break;
    case GPT_CHNL_TIMER1:
        baseAdr = GPT_1_BASE;
        break;
    case GPWT_CHNL_TIMER0:
        baseAdr = GPWT_0_BASE;
        break;
    }

    return baseAdr;
}

void GPT_Init(GPT_ConfigType *gptConfig)
{
    uint32 baseAdr;
    uint8 i = 0;

    for(i = 0; i < size; i++)
    {

        baseAdr = GPT_GetTimerBaseAdr(gptConfig[i].gptChannelid);

        /* Disable the timer before any operation */
        CLR_BIT(ACCESS_GPTMCTL(baseAdr), 0);

        if(gptConfig[i].type == No_Extensions)
        {
            /* Use the timer in 16 bit mode (or 32 bit mode in wide timers) */
            //ACCESS_GPTMCFG(baseAdr) &= 0xFFFFFFF1;
            ACCESS_GPTMCFG(baseAdr) |= 0x04;
        }
        else if (gptConfig[i].type == Extended)
        {
            /* Use the timer in 32 bit mode (or 64 bit mode in wide timers) */
          //  ACCESS_GPTMCFG(baseAdr) &= 0xFFFFFFF1;
            ACCESS_GPTMCFG(baseAdr) |= 0x0;
        }
        else if (gptConfig[i].type == RTC)
        {
            /*For a 16/32-bit timer, this value selects the 32-bit real-time clock (RTC) counter configuration*/
            /*For a 32/64-bit timer, this value selects the 64-bit real-time clock (RTC) counter configuration*/
        //    ACCESS_GPTMCFG(baseAdr) &= 0xFFFFFFF1;
            ACCESS_GPTMCFG(baseAdr) |= 0x1;
        }
        
        /* Set the timer mode */
        CLR_BIT(ACCESS_GPTMTAMR(baseAdr), 4);  /* Set to count down mode*/

        if(gptConfig[i].channelMode == GPT_CH_MODE_CONTINUOUS)
            ACCESS_GPTMTAMR(baseAdr) |= 0x02;  /* Set to periodic mode*/
        else if(gptConfig[i].channelMode == GPT_CH_MODE_ONESHOT)
            ACCESS_GPTMTAMR(baseAdr) |= 0x01;  /* Set to one shot mode*/
        else if (gptConfig[i].channelMode == GPT_CH_MODE_CAPTURE_MODE)
            ACCESS_GPTMTAMR(baseAdr) |= 0x3; /* Set to capture mode*/

        /* Set the prescale value */
        //ACCESS_GPTMTAPR(baseAdr) = gptConfig->prescaler;
        if(i == 0)
        {
            SET_BIT(ACCESS_GPTMIMR(baseAdr) , 0);
        }

    }

}
void GPT_StartTimer(const GPT_ChannelType gptChannelid, const GPT_ValueType value)
{
    uint32 baseAdr;
    baseAdr = GPT_GetTimerBaseAdr(gptChannelid);

    /* Disable the timer before any operation */
    CLR_BIT(ACCESS_GPTMCTL(baseAdr), 0u);

    /*Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).*/
    ACCESS_GPTMTAILR(baseAdr) =  value;

    /*Set the TnEN bit in the GPTMCTL register to enable the timer and start counting.*/
    SET_BIT(ACCESS_GPTMCTL(baseAdr), 0u);
    
}
void GPT_EnableNotification(void (*fun_ptr)(void), GPT_ChannelType gptChannelid)
{

}

void GPT_Init_PWM(GPT_ChannelType gptChannelid , u32 on_time)
{
    uint32 baseAdr = GPT_GetTimerBaseAdr(gptChannelid);

    /* Disable the timer before any operation */
    CLR_BIT(ACCESS_GPTMCTL(baseAdr), 0u);
    /*Configure PWM*/
    ACCESS_GPTMCFG(baseAdr) |= 0x04;
    /*In the GPTM Timer Mode (GPTMTnMR) register, set the TnAMS bit to 0x1*/
    SET_BIT(ACCESS_GPTMTAMR(baseAdr), 3u);
    /*In the GPTM Timer Mode (GPTMTnMR) register,set the TnCMR bit to 0x0*/
    CLR_BIT(ACCESS_GPTMTAMR(baseAdr), 2u);

    /*In the GPTM Timer Mode (GPTMTnMR) register,set the TnMR field to 0x2*/
    ACCESS_GPTMTAMR(baseAdr) |=0x2;
    /*Configure the output state of the PWM signal (whether or not it is inverted) in the TnPWML field of the GPTM Control (GPTMCTL) register.*/
    CLR_BIT(ACCESS_GPTMCTL(baseAdr) , 6u);

    /*Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.*/
    ACCESS_GPTMTAILR(baseAdr) = 50000;

    /*Load the GPTM Timer n Match (GPTMTnMATCHR) register with the match value.*/
    ACCESS_GPTMTAMATCHR(baseAdr) = on_time;

    /*Set the TnEN bit in the GPTMCTL register to enable the timer and start counting.*/
    SET_BIT(ACCESS_GPTMCTL(baseAdr), 0u);
    /*Polling on match flag*/
    //while((READ_BIT(ACCESS_GPTMRIS(baseAdr) , 4u)) == 0);

    /*clear flag*/
    //SET_BIT(ACCESS_GPTMICR(baseAdr), 4u);


}

void GPT_Set_Callback(void (*pf) (void))
{
    if(NULL !=pf)
    {
       GPT_Timer0_CallBack = pf;
    }
}

void Timer0_timerA_Handler(void)
{
    GPT_Timer0_CallBack();
    uint32 baseAdr = GPT_GetTimerBaseAdr(GPT_CHNL_TIMER0);
    (*((volatile u32 *)0x40030024)) = 0xFFFFFFFF;
    SET_BIT(ACCESS_GPTMICR(baseAdr), 0u);
}
