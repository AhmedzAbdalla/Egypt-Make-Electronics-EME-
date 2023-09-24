

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
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include <string.h>
#include "hw_gpio.h"
#include "LIB/BIT_MATH.h"
#include "FreeRTOS.h"
#include "Include/task.h"
#include "Include/queue.h"
#include "Include/projdefs.h"

/*user implementation for vApplicationIdleHook*/
void vApplicationIdleHook( void )
{
    while(1)
    {

    }
}

typedef struct
{
    uint32 sensor_ID ;
    uint32_t Sensor_Value;
}sensor_t;

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
                 /*Do whatever you need*/
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
            /*Do whatever you need*/
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
            }
        vTaskDelay(500);
        sensor.Sensor_Value += 100;
        }
}

int main(void)
{

    sensor_msg_queue = xQueueCreate( 4 , sizeof(sensor_t));

    xTaskCreate(&task_queue_Receive , " task_queue_Receive" , configMINIMAL_STACK_SIZE ,NULL , 2 , NULL );

    xTaskCreate(&task_queue_Send, " task_queue_Send" , configMINIMAL_STACK_SIZE ,NULL , 3 , NULL );
    
    xTaskCreate(&task_queue_Send_2, " task_queue_Send_2" , configMINIMAL_STACK_SIZE ,NULL , 3 , NULL );

    vTaskStartScheduler();
    while (1)
    {

    }
}

