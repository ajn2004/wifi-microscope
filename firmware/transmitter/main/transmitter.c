/*
  transmitter main
  This will be the main code for the transmitter chip, establishing what signal gets sent and how often. 
*/
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESPNOW_MAXDELAY 512
#define DUTY_CYCLE_PERIOD_MS 100

/* WiFi should start before using ESPNOW */
static void initialize_wifi(void)
{
  // System infra
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // initialize the driver
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

  // Configure the wifi interface
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(ESPNOW_WIFI_MODE) );

  // Activate the Wi-Fi and select a channel
  ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK( esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));
  
#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
  ESP_ERROR_CHECK( esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR) );
#endif
}

void transmit_frame(){}

void app_main(void)
{
  // Initialize the wifi configuration
  initialize_wifi();
  // initialize the bespoke transmistter setup
  //transmitter_init();

  while (true) {
    transmit_frame();
    vTaskDelay(pdMS_TO_TICKS(DUTY_CYCLE_PERIOD_MS));
  };
}
