#pragma once


#include "stdint.h"
#include "stm32f1xx_hal.h"


#define LA_NUM_DIODES 32
#define LA_NUM_BRIGHTNESS_LEVELS 64
#define LA_TX_DATA_LENGTH (LA_NUM_BRIGHTNESS_LEVELS - 1)


typedef uint32_t la_diodes_t;
typedef uint64_t la_brightness_pattern_t;


typedef struct {
    // required peripherals
    SPI_HandleTypeDef *hspi;
    TIM_HandleTypeDef *htim;
    uint32_t rclk_channel;
} LaserArray_Config_t;

typedef struct {
    LaserArray_Config_t config;

    uint8_t brightnesses[LA_NUM_DIODES];
    la_diodes_t tx_data[LA_TX_DATA_LENGTH];
} LaserArray_t;


HAL_StatusTypeDef LaserArray_Init(LaserArray_t *la, const LaserArray_Config_t *config);
HAL_StatusTypeDef LaserArray_SetBrightness(LaserArray_t *la, uint8_t diode, uint8_t brightness);
