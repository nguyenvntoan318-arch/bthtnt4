#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

/* =========================================================
 * STM32F103 GPIOA registers
 * ========================================================= */

#define PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE     (PERIPH_BASE + 0x20000UL)

#define RCC_BASE           (AHBPERIPH_BASE + 0x00001000UL)
#define GPIOA_BASE         (APB2PERIPH_BASE + 0x00000800UL)

#define RCC_APB2ENR        (*(volatile uint32_t *)(RCC_BASE + 0x18UL))
#define GPIOA_CRL          (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_ODR          (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))

#define RCC_APB2ENR_IOPAEN (1UL << 2)

/* LED pins */
#define LED1_PIN           (1UL << 0)   /* PA0 */
#define LED2_PIN           (1UL << 1)   /* PA1 */
#define LED3_PIN           (1UL << 2)   /* PA2 */


/* =========================================================
 * GPIO initialization
 * PA0, PA1, PA2 = Output Push-Pull 2 MHz
 * ========================================================= */

static void GPIO_Init(void)
{
    /* Enable GPIOA clock */
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;

    /*
     * PA0 = CNF=00, MODE=10
     * PA1 = CNF=00, MODE=10
     * PA2 = CNF=00, MODE=10
     */
    GPIOA_CRL &= ~(
        (0xFUL << 0) |
        (0xFUL << 4) |
        (0xFUL << 8)
    );

    GPIOA_CRL |= (
        (0x2UL << 0) |
        (0x2UL << 4) |
        (0x2UL << 8)
    );

    /* Turn all LEDs off initially */
    GPIOA_ODR &= ~(LED1_PIN | LED2_PIN | LED3_PIN);
}


/* =========================================================
 * LED Blink function
 * ========================================================= */

static void LED_Blink(uint32_t pin, uint32_t half_period_ms)
{
    GPIOA_ODR ^= pin;

    vTaskDelay(pdMS_TO_TICKS(half_period_ms));
}


/* =========================================================
 * Task 1
 * PA0 = 0.1 Hz
 *
 * Full period = 10 seconds
 * Half period = 5 seconds
 * ========================================================= */

static void LED_Task_01(void *argument)
{
    (void)argument;

    for (;;)
    {
        LED_Blink(LED1_PIN, 5000);
    }
}


/* =========================================================
 * Task 2
 * PA1 = 1 Hz
 *
 * Full period = 1 second
 * Half period = 500 ms
 * ========================================================= */

static void LED_Task_02(void *argument)
{
    (void)argument;

    for (;;)
    {
        LED_Blink(LED2_PIN, 500);
    }
}


/* =========================================================
 * Task 3
 * PA2 = 10 Hz
 *
 * Full period = 100 ms
 * Half period = 50 ms
 * ========================================================= */

static void LED_Task_03(void *argument)
{
    (void)argument;

    for (;;)
    {
        LED_Blink(LED3_PIN, 50);
    }
}


/* =========================================================
 * FreeRTOS hooks
 * ========================================================= */

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();

    for (;;)
    {
    }
}


void vApplicationStackOverflowHook(
    TaskHandle_t xTask,
    char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;

    taskDISABLE_INTERRUPTS();

    for (;;)
    {
    }
}


/* =========================================================
 * MAIN
 * ========================================================= */

int main(void)
{
    GPIO_Init();

    /* Create 3 independent LED tasks */

    xTaskCreate(
        LED_Task_01,
        "LED01",
        128,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        LED_Task_02,
        "LED02",
        128,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        LED_Task_03,
        "LED03",
        128,
        NULL,
        1,
        NULL
    );

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    for (;;)
    {
    }
}
