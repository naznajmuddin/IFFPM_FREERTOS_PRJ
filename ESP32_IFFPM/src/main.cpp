/**
 * @file main.cpp
 * @brief main ESP32
 * @copyright SPECTRE
 * @license Apache-2.0
 * @author Nazwa Najmuddin <naznajmuddin@gmail.com>
 */

#define BLYNK_TEMPLATE_ID "TMPL6AoOuAMW-"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "luf0lg1l9uZifSeHyjM-WQUcg0nYs_bx"

#define SPEED_MAX 255
#define SPEED_MIN 0

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "mdd10a.h"

AsyncWebServer server(80);

#define BLINK_GPIO GPIO_NUM_2
<<<<<<< HEAD
#define LEDPIN GPIO_NUM_13
#define BLOWER GPIO_NUM_18
#define EXTRUDER GPIO_NUM_19
=======
#define LEDPIN GPIO_NUM_2
>>>>>>> development

/** Structs **/
TaskHandle_t myThreadIndicator1 = NULL;
TaskHandle_t myLEDControlTask = NULL; // Task for LED control

WiFiUDP ntpUDP;
const long int utcOffsetInSeconds = 28800L; // UTC+8 Malaysia
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

int startTime;
int endTime;
int previousPinState = -1;

/** WIFI Manager **/
<<<<<<< HEAD
const char *ssid = "KTB 1 padu";
const char *pass = "ktb@1sahaja";
=======
const char *ssid = "kediamantunasbudi@unifi";
const char *pass = "ktb123456";
>>>>>>> development

/** Functions **/
void thread_indicator1(void *pvParameters);
void project_init();
void LEDControlTask(void *pvParameters); // Declaration of LED control task

/**
 * @brief Initialising tasks, connecting to Blynk server
 * @details Connecting to Blynk server, insert correct WiFI SSID & Password
 */
void setup()
{
    Serial.begin(115200);
    delay(100);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("\n\x1b[31m[WIFI_MANAGER] : Connecting\x1b[0m");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\n\x1b[31m[WIFI_MANAGER] : Connected to the WiFi network\x1b[0m");
    Serial.print("\n\x1b[31m[WIFI_MANAGER] : Local ESP32 IP: \x1b[0m");
    Serial.println(WiFi.localIP());

    WebSerial.begin(&server);
    server.begin();

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    timeClient.begin();

    project_init();

    if (Blynk.connected())
    {
        Serial.println("\x1b[32m[BLYNK_MANAGER] : Connected to Blynk Server\x1b[0m");
        WebSerial.println("[BLYNK_MANAGER] : Connected to Blynk Server");
    }

    pinMode(LEDPIN, OUTPUT);
    pinMode(BLOWER, OUTPUT);
    pinMode(EXTRUDER, OUTPUT);
    // Blynk.virtualWrite(V0, LOW);
    // Blynk.syncVirtual(V0);

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

    // Create the LED control task
    xTaskCreate(LEDControlTask, "LED Control", 4096, NULL, 2, &myLEDControlTask);
}

void project_init()
{
    Serial.println("\x1b[31m[JEBAT] : Initialising SPECTRE FIRMWARE...\x1b[0m");
    WebSerial.println("[JEBAT] : Initialising SPECTRE FIRMWARE...");
    Serial.println("\x1b[32m[PROJECT_NAME] : UMPSA IDP SPECTRE FIRMWARE\x1b[0m");
    WebSerial.println("[PROJECT_NAME] : UMPSA IDP SPECTRE FIRMWARE");
    Serial.println("\x1b[32m[MCU_MAKER] : Espressif\x1b[0m");
    WebSerial.println("[MCU_MAKER] : Espressif");
    Serial.println("\x1b[32m[MCU_PARTNUMBER] : ESP32\x1b[0m");
    WebSerial.println("[MCU_PARTNUMBER] : ESP32");
}

BLYNK_WRITE(V1)
{
    startTime = param[0].asInt();
    endTime = param[1].asInt();

    if (startTime == 0 && endTime == 0)
    {
        startTime = 999999;
        endTime = 999999;
    }

    char startTimeStr[10];
    sprintf(startTimeStr, "%02d:%02d %s", (startTime / 3600) % 12, (startTime / 60) % 60, (startTime >= 43200) ? "PM" : "AM");

    char endTimeStr[10];
    sprintf(endTimeStr, "%02d:%02d %s", (endTime / 3600) % 12, (endTime / 60) % 60, (endTime >= 43200) ? "PM" : "AM");

    Serial.println("[FEEDER_MANAGER] : Feed Time Input Received SUCCESS!");
    WebSerial.println("[FEEDER_MANAGER] : Feed Time Input Received SUCCESS!");
    Blynk.virtualWrite(V2, "[FEEDER_MANAGER] : Feed Time Input Received SUCCESS!");
    Serial.print("[FEEDER_MANAGER] : Start Time: ");
    Serial.println(startTimeStr);
    WebSerial.print("[FEEDER_MANAGER] : Start Time: ");
    WebSerial.println(startTimeStr);
    Blynk.virtualWrite(V2, "[FEEDER_MANAGER] : Start Time: ");
    Blynk.virtualWrite(V2, startTimeStr);
    Serial.print("[FEEDER_MANAGER] : End Time: ");
    Serial.println(endTimeStr);
    WebSerial.print("[FEEDER_MANAGER] : End Time: ");
    WebSerial.println(endTimeStr);
    Blynk.virtualWrite(V2, "[FEEDER_MANAGER] : End Time: ");
    Blynk.virtualWrite(V2, endTimeStr);
}

/**
 * @brief Blinking LED1 as a thread indicator and PCB is working
 * @return PCB LED1 blinking
 */
void thread_indicator1(void *pvParameters)
{
    Serial.println("\x1b[33m[THREAD_INDICATOR] : Thread Started and RUNNING\x1b[0m");
    WebSerial.println("[THREAD_INDICATOR] : Thread Started and RUNNING");

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

/**
 * @brief Controlling components through time input
 */
void LEDControlTask(void *pvParameters)
{
    motor grinder;
    motor crusher;

    grinder.init_data(25, 26, 0);
    crusher.init_data(22, 23, 1);

    bool extruderOn = false;
    bool blowerOn = false;

    for (;;)
    {
        timeClient.update();
        int HH = timeClient.getHours();
        int MM = timeClient.getMinutes();
        int SS = timeClient.getSeconds();
        int serverTime = 3600 * HH + 60 * MM + SS;
        int pinState = digitalRead(LEDPIN);

        char startTimeStr[10];
        sprintf(startTimeStr, "%02d:%02d %s", (startTime / 3600) % 12, (startTime / 60) % 60, (startTime >= 43200) ? "PM" : "AM");

        char endTimeStr[10];
        sprintf(endTimeStr, "%02d:%02d %s", (endTime / 3600) % 12, (endTime / 60) % 60, (endTime >= 43200) ? "PM" : "AM");

        if (startTime <= serverTime && serverTime <= endTime)
        {
            digitalWrite(LEDPIN, 1);   // Turn on LED
            digitalWrite(EXTRUDER, 1); // Turn on EXTRUDER
            delay(1000);
            digitalWrite(EXTRUDER, 0);
            digitalWrite(BLOWER, 1);
            delay(1000);
            digitalWrite(BLOWER, 0); // Turn on BLOWER
            // Serial.println("[FEEDER_MANAGER] : Motor STARTED!");
            grinder.set_direction(LOW);
            grinder.set_spin(SPEED_MAX);
            crusher.set_direction(LOW);
            crusher.set_spin(SPEED_MAX);
        }
        else if (startTime != serverTime && serverTime != endTime)
        {
            digitalWrite(LEDPIN, 0); // Turn off LED
            // Serial.println("[FEEDER_MANAGER] : Motor STOPPED!");
            grinder.set_spin(SPEED_MIN);
            crusher.set_spin(SPEED_MIN);
        }

        if (pinState != previousPinState)
        {
            // Print the message when the state changes
            if (pinState == 1)
            {
                Serial.print("[FEEDER_MANAGER] : Motor STARTED! : ");
                Serial.println(startTimeStr);
                WebSerial.print("[FEEDER_MANAGER] : Motor STARTED! : ");
                WebSerial.println(startTimeStr);
            }
            else if (pinState == 0)
            {
                Serial.print("[FEEDER_MANAGER] : Motor STOPPED! : ");
                Serial.println(endTimeStr);
                WebSerial.print("[FEEDER_MANAGER] : Motor STOPPED! : ");
                WebSerial.println(endTimeStr);
            }

            // Update the previous pin state
            previousPinState = pinState;
        }

        if (pinState == 1)
        {
            // Serial.print("[FEEDER_MANAGER] : Motor STARTED! : ");
            // Serial.println(startTimeStr);
            WebSerial.print("[FEEDER_MANAGER] : Motor STARTED! : ");
            WebSerial.println(startTimeStr);
        }
        else if (pinState == 0)
        {
            // Serial.print("[FEEDER_MANAGER] : Motor STOPPED! : ");
            // Serial.println(endTimeStr);
            WebSerial.print("[FEEDER_MANAGER] : Motor STOPPED! : ");
            WebSerial.println(endTimeStr);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}

void loop()
{
    Blynk.run();
}
