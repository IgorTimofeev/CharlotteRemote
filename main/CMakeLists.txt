file(GLOB_RECURSE PIZDA_SOURCES
     "*.*"
)

idf_component_register(
    SRCS ${PIZDA_SOURCES}
    PRIV_REQUIRES spi_flash
    INCLUDE_DIRS ""
    REQUIRES esp_timer driver mbedtls yoba
)
