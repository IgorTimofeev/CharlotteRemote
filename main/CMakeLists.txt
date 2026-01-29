file(GLOB_RECURSE pizda_sources
    "*.*"
)

idf_component_register(
    SRCS ${pizda_sources}
    INCLUDE_DIRS "."
    REQUIRES
        esp_timer
        esp_driver_ledc
        driver
        nvs_flash
        esp_adc
        YOBA
        ESP-IDF-SX1262
        ESP-IDF-NVSExtensions
        ESP-IDF-BitStream
        ESP-IDF-Units
        ESP-IDF-LowPassFilter
        ESP-IDF-PIDController
        ESP-IDF-Battery
)

include_directories(.)