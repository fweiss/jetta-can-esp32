#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/can.h"

/* --------------------- Definitions and static variables ------------------ */
//Example Configuration
#define PING_PERIOD_MS          250
#define NO_OF_DATA_MSGS         50
#define NO_OF_ITERS             3
#define ITER_DELAY_MS           1000
#define RX_TASK_PRIO            8
#define TX_TASK_PRIO            9
#define CTRL_TSK_PRIO           10
#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define EXAMPLE_TAG             "CAN Master"

static const can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
static const can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, CAN_MODE_NORMAL);

static can_message_t tach_message = {.identifier = 0x280, .data_length_code = 8,
                                           .flags = CAN_MSG_FLAG_NONE, .data = {0, 0 , 10 , 10 ,0 ,0 ,0 ,0}};

/* --------------------------- Tasks and Functions -------------------------- */

static void can_receive_task(void *arg) {
    while (1) {
        can_message_t rx_msg;
        can_receive(&rx_msg, portMAX_DELAY);
        uint32_t data = 0;
        for (int i = 0; i < rx_msg.data_length_code; i++) {
            data |= (rx_msg.data[i] << (i * 8));
        }
        ESP_LOGI(EXAMPLE_TAG, "Received data value %04x: %d", rx_msg.identifier, data);
    }
    vTaskDelete(NULL);
}

// fixme run at timer
static void can_transmit_task(void *arg) {
    while (1) {
        ESP_LOGI(EXAMPLE_TAG, "sending tach");

        const uint16_t rpm = 2000;
        uint16_t scaledRpm = rpm * 4;
        tach_message.data[2] = scaledRpm & 0xff;
        tach_message.data[3] = scaledRpm >> 8;
        can_transmit(&tach_message, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(PING_PERIOD_MS));
    }
    vTaskDelete(NULL);
}

void app_main()
{
    //Install CAN driver
    ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");
    ESP_ERROR_CHECK(can_start());
    ESP_LOGI(EXAMPLE_TAG, "Driver started");


    ESP_LOGI(EXAMPLE_TAG, "starting receive task");
    xTaskCreatePinnedToCore(can_receive_task, "CAN_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
    ESP_LOGI(EXAMPLE_TAG, "starting transsmit task");
    xTaskCreatePinnedToCore(can_transmit_task, "CAN_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
}
