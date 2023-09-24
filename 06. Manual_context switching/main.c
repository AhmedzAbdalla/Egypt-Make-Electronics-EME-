#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "hw_gpio.h"
#include "LIB/BIT_MATH.h"
#include "driverlib/fpu.h"


volatile uint32_t counter = 0;


void task_1(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    while(1)
    {
        if(counter % 5 == 0)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // Turn on blue LED
        }
    }
}
void task_2(void)
{
    while(1)
    {
        if(counter % 10 == 0)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on red LED
        }
    }
}

/*array which SP will point to maually*/

volatile uint32_t stack_BlueBlinky[10] = {0x00000000,&task_1,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD};
volatile uint32_t *sp_BlueBlinky = &stack_BlueBlinky[0];

int main(void)
{
    /*initialize system clock*/
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysTickPeriodSet(SysCtlClockGet() / 1000 * 100); // Set interrupt interval to 1ms
    SysTickIntEnable(); // Enable SysTick interrupt
    IntMasterEnable(); // Enable interrupt
    SysTickEnable();// start SysTick
    FPUDisable(); /*Disable FPU is must for this demo*/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);/*Enable clock for port f*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);/*init GPIO_pin_1 GPIO_PIN_2as output*/
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Turn off red LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn off blue LED


    task_2();


    while(1)
    {

    }

	return 0;
}

/*when the program haults in the break pint at line of incrementing counter you
 * should go to SP and assign thesp_BlueBlinky to it,
    the first element of array will be popped and stored in PSR REG and the second element
    will be popped and stored in PC which means it will jump to task_1 and blue led will be turned on*/
void SysTick_Handler(void)
{
    counter++;
    SysTickPeriodSet(SysCtlClockGet() / 1000 * 100); // Set interrupt interval to 1ms
}
