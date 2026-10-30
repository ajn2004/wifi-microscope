/*
  reciever main
  This will be the main code for the reciever chip
*/
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define WIFI_MODE WIFI_MODE_STA
#define WIFI_CHANNEL 6

typedef struct {
  uint8_t source_mac[6];
  int8_t rssi;
  uint8_t channel;
  uint32_t timestamp;
} rx_measurement_t;

static QueueHandle_t rx_queue;

static void initialize_wifi(void) {
  // System infra
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // initialize the driver
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Configure the wifi interface
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE));

  // Activate the Wi-Fi and select a channel
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
}

static void receive_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  // anything to recieve?
  if (buf == NULL || type != WIFI_PKT_MGMT) {
    return;
  }

  const wifi_promiscuous_pkt_t *packet = (const wifi_promiscuous_pkt_t *)buf;

  if (packet->rx_ctrl.sig_len < 24) {
    return;
  }

  const uint8_t *frame = packet->payload;

  if (frame[0] != 0x40) {
    return;
  }

  rx_measurement_t measurement = {
    .rssi = packet->rx_ctrl.rssi,
    .channel = packet->rx_ctrl.channel,
    .timestamp = packet->rx_ctrl.timestamp,
  };
  memcpy(measurement.source_mac, &frame[10], sizeof(measurement.source_mac));
  xQueueSend(rx_queue, &measurement, 0);
}

void app_main(void) {
  // initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
    ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // initialize wifie
  initialize_wifi();

  rx_queue = xQueueCreate(32, sizeof(rx_measurement_t));
  configASSERT(rx_queue != NULL);

  // accept management frames
  wifi_promiscuous_filter_t filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT};

  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));

  // Register our callback
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(receive_callback));

  // Begin receiving frames in promiscuous mode
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));

  rx_measurement_t measurement;
  while (true) {
    if (xQueueReceive(rx_queue, &measurement, portMAX_DELAY) == pdTRUE) {
      ESP_LOGI("RX",
        "probe received, source=%02x:%02x:%02x:%02x:%02x:%02x, RSSI=%d dBm, "
        "ch=%u, t=%" PRIu32,
        measurement.source_mac[0], measurement.source_mac[1],
        measurement.source_mac[2], measurement.source_mac[3],
        measurement.source_mac[4], measurement.source_mac[5], measurement.rssi,
        measurement.channel, measurement.timestamp);
    }
  }
}
