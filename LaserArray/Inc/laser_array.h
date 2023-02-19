#pragma once


#include "main.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"


#define LA_NUM_DIODES 32
#define LA_NUM_BRIGHTNESS_LEVELS 64
#define LA_TX_DATA_LENGTH (LA_NUM_BRIGHTNESS_LEVELS - 1)

#ifndef LA_FADE_TICK_RATE
    #warning "LA_FADE_TICK_RATE was not defined! Using default setting"
    #define LA_FADE_TICK_RATE 60
#endif


typedef uint32_t la_bitmask_t;
typedef uint64_t la_brightness_pattern_t;


typedef struct {
    // required peripherals
    SPI_HandleTypeDef *hspi;
    TIM_HandleTypeDef *htim_transfer;
    TIM_HandleTypeDef *htim_fade;
    uint32_t rclk_channel;
} LaserArray_Config_t;

typedef struct {
    uint8_t current_brightness;
    uint8_t source_brightness;
    uint8_t target_brightness;

    uint32_t transition_duration;
    uint32_t transition_tick;
} LaserArray_Diode_t;

typedef struct {
    LaserArray_Config_t config;

    LaserArray_Diode_t diodes[LA_NUM_DIODES];
    la_bitmask_t tx_data[LA_TX_DATA_LENGTH];
} LaserArray_t;


HAL_StatusTypeDef LaserArray_Init(LaserArray_t *la, const LaserArray_Config_t *config);
HAL_StatusTypeDef LaserArray_SetBrightness(LaserArray_t *la, uint8_t diode_index, uint8_t brightness);
HAL_StatusTypeDef LaserArray_FadeBrightness(LaserArray_t *la, uint8_t diode_index, uint8_t brightness, uint32_t duration);

HAL_StatusTypeDef LaserArray_TIM_PeriodElapsedHandler(LaserArray_t *la, TIM_HandleTypeDef *htim);
