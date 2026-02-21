file(GLOB_RECURSE pizda_sources
    "*.*"
)

idf_component_register(
    SRCS ${pizda_sources}
    INCLUDE_DIRS "."
    REQUIRES
        esp_driver_ledc
        nvs_flash
        esp_adc
        YOBA
        ESP-IDF-SX1262
        ESP-IDF-NVSExtensions
        ESP-IDF-BitStream
        ESP-IDF-Units
        ESP-IDF-LowPassFilter
        ESP-IDF-PIDController
        ESP-IDF-CircularQueue
        ESP-IDF-Battery
        ESP-IDF-BuzzerAudio
)

include_directories(.)