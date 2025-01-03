/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "demo_hw.h"
#include <string.h>

#define DEFAULT_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define DEFAULT_TASK_STACK_SIZE 1024

#define NUM_OF_PRINT_MSG_TASK (5u)

static SemaphoreHandle_t _semphr = NULL;

void _setup(void * pvParameters);
static void _printMsg(void * pvParameters);

void demo_init(void) {
    xTaskCreate(_setup,                      /* The function that implements the task. */
                "S",                        /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                DEFAULT_TASK_STACK_SIZE, /* The size of the stack to allocate to the task. */
                NULL,                       /* The parameter passed to the task */
                DEFAULT_TASK_PRIORITY,   /* The priority assigned to the task. */
                NULL );                     /* The task handle is not required, so NULL is passed. */

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }
}

// simulating arduino environment
void _setup(void * pvParameters) {
    (void)pvParameters;
    char msg[] = "Hi from _setup\'s stack\n";
    char taskNames[NUM_OF_PRINT_MSG_TASK][2] = {"A", "B", "C", "D", "E"};
    uint8_t i;

    _semphr = xSemaphoreCreateCounting(NUM_OF_PRINT_MSG_TASK, 0);

    for(i = 0; i < NUM_OF_PRINT_MSG_TASK; ++i) {
        xTaskCreate(_printMsg,               /* The function that implements the task. */
                    taskNames[i],                    /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    DEFAULT_TASK_STACK_SIZE, /* The size of the stack to allocate to the task. */
                    msg,                     /* The parameter passed to the task */
                    DEFAULT_TASK_PRIORITY,   /* The priority assigned to the task. */
                    NULL );                     /* The task handle is not required, so NULL is passed. */
    }

    for(i = 0; i < NUM_OF_PRINT_MSG_TASK; ++i) {
        xSemaphoreTake(_semphr, portMAX_DELAY);
    }

    demo_hw_term_writeLine("_setup done");
    vTaskDelete(NULL);
}

static void _printMsg( void * pvParameters ) {
    configASSERT(!(_semphr == NULL));
    configASSERT(!(pvParameters == NULL));
    char msg[100];
    strcpy(msg, (char *)pvParameters);
    demo_hw_term_printf("%s", msg);
    xSemaphoreGive(_semphr);
    vTaskDelete(NULL);
}