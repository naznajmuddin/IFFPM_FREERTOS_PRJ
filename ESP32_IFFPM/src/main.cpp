/**
 * @file main.cpp
 * @brief main ESP32
 * @copyright SPECTRE
 * @license Apache-2.0
 * @author Nazwa Najmuddin <naznajmuddin@gmail.com>
 */

#include <Arduino.h>
#include "driver/gpio.h"
#include "sdkconfig.h"

#define BLINK_GPIO GPIO_NUM_2

TaskHandle_t myThreadIndicator1 = NULL;

void thread_indicator1(void *pvParameters);

/**
 * @brief Initialising tasks, connecting to Blynk server
 * @details Connecting to Blynk server, insert correct WiFI SSID & Password
 */
void setup()
{
    Serial.begin(115200);

    /**
     * @brief Creating tasks to run simultaneously according to priority
     * @details xTaskCreate(
     *                           TaskFunction_t pvTaskCode, [name of task function]
     *                           const char * const pcName, [messages]
     *                           const uint32_t usStackDepth, [Words, not bytes!]
     *                          void * const pvParameters, [usually NULL]
     *                           UBaseType_t uxPriority, [priority level (1(LOWEST) to 15(HIGHEST))]
     *                          TaskHandle_t * const pxCreatedTask) PRIVILEGED_FUNCTION
     */
    xTaskCreate(thread_indicator1, "Blinking LED", 4096, NULL, 1, &myThreadIndicator1);
}

/**
 * @brief Blinking LED1 as a thread indicator and PCB is working
 * @return PCB LED1 blinking
 */
void thread_indicator1(void *pvParameters)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    for (;;)
    {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void loop()
{
    
}