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
#include "esp_log.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESPNOW_MAXDELAY 512
#define DUTY_CYCLE_PERIOD_MS 100
#define WIFI_MODE WIFI_MODE_STA
#define WIFI_CHANNEL 6

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
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE) );

  // Activate the Wi-Fi and select a channel
  ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK( esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
  
}

static esp_err_t transmit_frame(void){
  uint8_t frame[] = {
        // 802.11 MAC header (24 bytes)

        0x40, 0x00,  // Frame Control: probe request
        0x00, 0x00,  // Duration

        // Address 1: destination (broadcast)
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

        // Address 2: source MAC (filled in below)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        // Address 3: BSSID (wildcard)
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

        0x00, 0x00,  // Sequence Control

        // Frame body: 802.11 information elements

        0x00, 0x00,  // SSID: zero-length, wildcard

        // Supported Rates: 1, 2, 5.5, 11 Mbps
        0x01, 0x04, 0x82, 0x84, 0x8b, 0x96
    };
  uint8_t mac[6];
  esp_err_t err = esp_wifi_get_mac(WIFI_IF_STA, mac);
  if (err != ESP_OK){
    return err;
  }

  memcpy(&frame[10], mac, 6);
  
  err = esp_wifi_80211_tx(
                          WIFI_IF_STA,
                          frame,
                          sizeof(frame),
                          true
                         );
  return err;
}

void app_main(void)
{
  // initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
    ESP_ERROR_CHECK( nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  //initialize wifie
  initialize_wifi();
  
  // initialize the bespoke transmistter setup
  //transmitter_init();

  while (true) {
    ESP_LOGI("TX", "Transmitter loop running");
    esp_err_t err = transmit_frame();
    if (err != ESP_OK){
      ESP_LOGI("TX", "ERROR");
    }
    vTaskDelay(pdMS_TO_TICKS(DUTY_CYCLE_PERIOD_MS));
  };
}
