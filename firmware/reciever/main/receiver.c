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
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define WIFI_MODE WIFI_MODE_STA
#define WIFI_CHANNEL 6
static const uint8_t TX_MAC[6] = {0x1c, 0xc3, 0xab, 0x3c, 0x96, 0x10};

typedef struct {
  uint8_t source_mac[6];
  int8_t rssi;
  uint8_t channel;
  uint32_t timestamp;
} rx_measurement_t;

#define CSI_MAX_BYTES 640

typedef struct {
  uint8_t source_mac[6];
  uint32_t timestamp;
  int8_t rssi;
  uint8_t channel;
  uint8_t sig_mode;
  uint8_t rate_code;
  bool first_word_invalid;
  uint16_t len;
  uint8_t bytes[CSI_MAX_BYTES];
} csi_record_t;

static QueueHandle_t csi_queue;

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

  if (memcmp(&frame[10], TX_MAC, sizeof(TX_MAC)) != 0) {
    return;
  }

  rx_measurement_t measurement = {
    .rssi = packet->rx_ctrl.rssi,
    .channel = packet->rx_ctrl.channel,
    .timestamp = packet->rx_ctrl.timestamp,
  };
  memcpy(measurement.source_mac, &frame[10], sizeof(measurement.source_mac));
  // Report to serial
  xQueueSend(rx_queue, &measurement, 0);
}

static void csi_receive_callback(void *ctx, wifi_csi_info_t *info) {
  (void)ctx;
  if (info == NULL || info->buf == NULL) {
    return;
  }

  // Only accept CSI from our transmitter
  if (memcmp(info->mac, TX_MAC, sizeof(TX_MAC)) != 0) {
    return;
  }

  // validate the sample length before copying
  if (info->len == 0 || info->len > CSI_MAX_BYTES) {
    return;
  }

  csi_record_t record = {
    .timestamp = info->rx_ctrl.timestamp,
    .rssi = info->rx_ctrl.rssi,
    .channel = info->rx_ctrl.channel,
    .sig_mode = info->rx_ctrl.sig_mode,
    .rate_code = info->rx_ctrl.rate,
    .first_word_invalid = info->first_word_invalid,
    .len = info->len,
  };

  // copy this into the out-read queue
  memcpy(record.source_mac, info->mac, 6);
  memcpy(record.bytes, info->buf, info->len);

  (void)xQueueSend(csi_queue, &record, 0);
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

  csi_queue = xQueueCreate(8, sizeof(csi_record_t));
  configASSERT(csi_queue != NULL);

  // accept management frames
  wifi_promiscuous_filter_t filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT};

  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));

  // Begin receiving frames in promiscuous mode
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));

  // Configure which training fields we want
  wifi_csi_config_t csi_cfg = {
    .lltf_en = true,
    .htltf_en = true,
    .stbc_htltf2_en = false,
    .ltf_merge_en = false,
    .channel_filter_en = false,
    .manu_scale = false,
    .shift = 0,
  };

  // Register our CSI callback
  ESP_ERROR_CHECK(esp_wifi_set_csi_rx_cb(csi_receive_callback, NULL));

  // Apply configuration
  ESP_ERROR_CHECK(esp_wifi_set_csi_config(&csi_cfg));

  // Start CSI capture
  ESP_ERROR_CHECK(esp_wifi_set_csi(true));

  ESP_LOGI("RX", "CSI capture enabled");

  csi_record_t sample;

  while (true) {
    if (xQueueReceive(csi_queue, &sample, portMAX_DELAY) != pdTRUE) {
      continue;
    }

    printf("CSI,%" PRIu32 ",%d,%u,%u,%u,%u,%u,", sample.timestamp,
      (int)sample.rssi, (unsigned)sample.channel, (unsigned)sample.sig_mode,
      (unsigned)sample.rate_code, (unsigned)sample.first_word_invalid,
      (unsigned)sample.len);

    // Raw CSI bytes, encoded as hexadecimal
    for (uint16_t i = 0; i < sample.len; i++) {
      printf("%02x", (unsigned)sample.bytes[i]);
    }

    putchar('\n');
  }
}
