

/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include <string.h>
#include "hw_gpio.h"
#include "LIB/BIT_MATH.h"
#include "driverlib/flash.h"
#include "driverlib/fpu.h"
#include "driverlib/can.h"
#include "include/FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "projdefs.h"
#include "Inc/APP/AutonomousCar/AutonomousCar.h"
void vApplicationIdleHook( void )
{
    while(1)
    {

    }
}

void Switch1IntHandler(void);

SemaphoreHandle_t semaphoreHandle;

BaseType_t state = 0;
// Task 1
void Task1(void *pvParameters)
{
    while (1)
    {
        BaseType_t status_1  =xSemaphoreTake(semaphoreHandle, 10);
        if ( status_1 == pdTRUE)
        {
            // Toggle LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_2) ^GPIO_PIN_2  );

            vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
        }
        else
        {
            /*Nothing*/
        }
        vTaskDelay(1500 / portTICK_PERIOD_MS);

    }
}
// Task 2
void Task2(void *pvParameters)
{
    while (1)
    {
        BaseType_t status_2  =xSemaphoreTake(semaphoreHandle, 10);
        if (status_2 == pdTRUE)
        {
            // Toggle LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_1) ^GPIO_PIN_1  );

        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE  , GPIO_PIN_4  );
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    GPIOPadConfigSet(GPIO_PORTF_BASE ,GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);

    GPIOIntRegister(GPIO_PORTF_BASE, Switch1IntHandler);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);

    // Enable the interrupt controller
IntMasterEnable();


     vSemaphoreCreateBinary(semaphoreHandle);



    xTaskCreate(&Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(&Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    vTaskStartScheduler();
    while (1)
    {

    }
}

void Switch1IntHandler(void)
{
    state = xSemaphoreGiveFromISR(semaphoreHandle , NULL);

    // Clear the interrupt flag for switch 1
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);

}

//    char statsBuffer[200];
//    TaskHandle_t taskHandle = /* Obtain the handle of the task you want to measure */;
//    ulTaskGetRunTimeStats(statsBuffer);

    //autonomous_car_init();


    // Create Task 1
    //xTaskCreate(&start_stop_condition, "start_stop_condition", configMINIMAL_STACK_SIZE, NULL, 4, &TaskHandle_t_start_stop_condition);

//    // Create Task 2
    //xTaskCreate(&avoid_obstacles, "avoid_obstacles", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
//
//    // create task 3
    //xTaskCreate(&ldr_swing_car, "ldr_swing_car", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
//
//    // create task 4
    //xTaskCreate(&get_temp, "get_temp", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
//
//    // create task 5
    //xTaskCreate(&lcd_display, "lcd_display", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

    // Start the FreeRTOS scheduler
    //vTaskStartScheduler();

    //lcd_send_chr(&lcd_car , 'd');
    // Should never reach here

/*
 *
 *
 *typedef struct
{
    uint32 sensor_ID ;
    uint32_t Sensor_Value;
}sensor_t;
// Task 2
void Task2(void *pvParameters)
{
    while (1)
    {
        // Toggle LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1s
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1s
    }
}


static QueueHandle_t sensor_msg_queue;

void task_queue_Receive(void)
{
    sensor_t rec_item;
    static BaseType_t status_recieve = 0;
    while(1)
        {
        status_recieve = xQueueReceive(sensor_msg_queue, (void*) &rec_item, 0);
        if(status_recieve == pdTRUE)
            {
                //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            }
        vTaskDelay(1000);
        }
}

void task_queue_Send(void)
{
    static sensor_t sensor = {.sensor_ID = 25, .Sensor_Value = 66};;
    static BaseType_t status_send = 0;
    while(1)
        {
        status_send = xQueueSend(sensor_msg_queue, (void*) &sensor, 0);
        if(status_send == pdTRUE)
            {
                //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            }
        vTaskDelay(500);
        sensor.Sensor_Value += 10;
        }
}

void task_queue_Send_2(void)
{
    static sensor_t sensor = {.sensor_ID = 11, .Sensor_Value = 66};;
    static BaseType_t status_send = 0;
    while(1)
        {
        status_send = xQueueSend(sensor_msg_queue, (void*) &sensor, 0);
        if(status_send == pdTRUE)
            {
                //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            }
        vTaskDelay(500);
        sensor.Sensor_Value += 100;
        }
}
 * */
