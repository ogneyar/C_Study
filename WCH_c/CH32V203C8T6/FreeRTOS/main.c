
// НЕ РАБОТАЕТ!!!

// но в MounRiver'e запускается

/*
 *@Note
 task1, task2 and task3 alternate printing
*/

#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      256
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      256
#define TASK3_TASK_PRIO     5
#define TASK3_STK_SIZE      256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;
TaskHandle_t Task3Task_Handler;


/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOB.0/1
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure={0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*********************************************************************
 * @fn      task1_task
 *
 * @brief   task1 program.
 *
 * @param  *pvParameters - Parameters point of task1
 *
 * @return  none
 */
void task1_task(void *pvParameters)
{
    while(1)
    {
        printf("task1 entry\r\n");
        GPIO_SetBits(GPIOB, GPIO_Pin_3);
        vTaskDelay(250);
        GPIO_ResetBits(GPIOB, GPIO_Pin_3);
        vTaskDelay(250);
    }
}

/*********************************************************************
 * @fn      task2_task
 *
 * @brief   task2 program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
void task2_task(void *pvParameters)
{
    while(1)
    {
        printf("task2 entry\r\n");
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
        vTaskDelay(500);
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
        vTaskDelay(500);
    }
}

/*********************************************************************
 * @fn      task3_task
 *
 * @brief   task3 program.
 *
 * @param  *pvParameters - Parameters point of task3
 *
 * @return  none
 */
void task3_task(void *pvParameters)
{
    while(1)
    {
        printf("task3 entry\r\n");
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        vTaskDelay(750);
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
        vTaskDelay(750);
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("FreeRTOS Kernel Version:%s\r\n",tskKERNEL_VERSION_NUMBER);

    GPIO_Toggle_INIT();
    /* create three task */   
    xTaskCreate((TaskFunction_t )task1_task,
                    (const char*    )"task1",
                    (uint16_t       )TASK1_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_TASK_PRIO,
                    (TaskHandle_t*  )&Task1Task_Handler);

    xTaskCreate((TaskFunction_t )task2_task,
                    (const char*    )"task2",
                    (uint16_t       )TASK2_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK2_TASK_PRIO,
                    (TaskHandle_t*  )&Task2Task_Handler);
    
    xTaskCreate((TaskFunction_t )task3_task,
                    (const char*    )"task3",
                    (uint16_t       )TASK3_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK3_TASK_PRIO,
                    (TaskHandle_t*  )&Task3Task_Handler);

    vTaskStartScheduler();

    while(1)
    {
        printf("shouldn't run at here!!\n");
    }
}

