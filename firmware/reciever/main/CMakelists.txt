idf_component_register(
    SRCS "receiver.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
        esp_wifi
        esp_event
        esp_netif
        nvs_flash
)
