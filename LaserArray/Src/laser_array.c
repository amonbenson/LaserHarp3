#include "laser_array.h"
#include "string.h"
#include "stdio.h"
#include "logging.h"


/*
Table generated using the following python snippet:

import math

N_LEVELS = 32
N_BITS = max(8, 2**math.ceil(math.log2(N_LEVELS - 1)))
GAMMA = 1.5

for n in range(N_LEVELS):
    pattern = 0

    b = round(n**GAMMA / N_LEVELS**(GAMMA-1))
    for i in range(b):
        shift = N_LEVELS * i // b
        pattern |= 1 << shift

    print(f"UINT{N_BITS}_C(0b{pattern :0{N_BITS}b}),")
 */
static la_brightness_pattern_t la_brightness_patterns[LA_NUM_BRIGHTNESS_LEVELS] = {
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000000),
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000000),
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000000),
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000001),
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000001),
        UINT64_C(0b0000000000000000000000000000000000000000000000000000000000000001),
        UINT64_C(0b0000000000000000000000000000000100000000000000000000000000000001),
        UINT64_C(0b0000000000000000000000000000000100000000000000000000000000000001),
        UINT64_C(0b0000000000000000000001000000000000000000001000000000000000000001),
        UINT64_C(0b0000000000000000000001000000000000000000001000000000000000000001),
        UINT64_C(0b0000000000000001000000000000000100000000000000010000000000000001),
        UINT64_C(0b0000000000001000000000000100000000000010000000000001000000000001),
        UINT64_C(0b0000000000001000000000000100000000000010000000000001000000000001),
        UINT64_C(0b0000000000100000000001000000000100000000001000000000010000000001),
        UINT64_C(0b0000000001000000001000000001000000001000000001000000001000000001),
        UINT64_C(0b0000000001000000001000000001000000001000000001000000001000000001),
        UINT64_C(0b0000000100000001000000010000000100000001000000010000000100000001),
        UINT64_C(0b0000000100000010000001000000100000010000001000000100000010000001),
        UINT64_C(0b0000001000001000000100000100000100000010000010000001000001000001),
        UINT64_C(0b0000001000001000000100000100000100000010000010000001000001000001),
        UINT64_C(0b0000010000010000010000010000010000100000100000100000100000100001),
        UINT64_C(0b0000010000100001000001000010000100000100001000010000010000100001),
        UINT64_C(0b0000100001000010000100001000010000100001000010000100001000010001),
        UINT64_C(0b0000100001000100001000100001000100001000010001000010001000010001),
        UINT64_C(0b0000100010001000010001000100010000100010001000100001000100010001),
        UINT64_C(0b0001000100010001000100010001000100010001000100010001000100010001),
        UINT64_C(0b0001000100010001001000100010001001000100010001001000100010001001),
        UINT64_C(0b0001000100100010010001001000100100010001001000100100010010001001),
        UINT64_C(0b0001001000100100100010010010001001000100100100010010010001001001),
        UINT64_C(0b0001001001001001000100100100100100010010010010010001001001001001),
        UINT64_C(0b0001001001001001001001001001001001001001001001001001001001001001),
        UINT64_C(0b0010010010010010010010010010010100100100100100100100100100100101),
        UINT64_C(0b0010010010010100100100100101001001001010010010010010100100100101),
        UINT64_C(0b0010010100100101001001010010010100100101001001010010010100100101),
        UINT64_C(0b0010010100101001010010010100101001010010100100101001010010100101),
        UINT64_C(0b0010100101001010010100101001010100101001010010100101001010010101),
        UINT64_C(0b0010100101010010101001010010101001010100101001010100101010010101),
        UINT64_C(0b0010101001010101001010100101010100101010010101010010101001010101),
        UINT64_C(0b0010101010010101010100101010101001010101010010101010100101010101),
        UINT64_C(0b0010101010101010010101010101010100101010101010100101010101010101),
        UINT64_C(0b0101010101010101010101010101010101010101010101010101010101010101),
        UINT64_C(0b0101010101010101010101010101010110101010101010101010101010101011),
        UINT64_C(0b0101010101010101101010101010101101010101010101011010101010101011),
        UINT64_C(0b0101010101101010101011010101010110101010101101010101011010101011),
        UINT64_C(0b0101010110101011010101011010101101010101101010110101010110101011),
        UINT64_C(0b0101011010110101101011010110101101010110101101011010110101101011),
        UINT64_C(0b0101101011010110101101101011010110101101011011010110101101011011),
        UINT64_C(0b0101101101011011010110110101101101011011010110110101101101011011),
        UINT64_C(0b0101101101101101101101101101101101011011011011011011011011011011),
        UINT64_C(0b0110110110110110110110110110110110110110110110110110110110110111),
        UINT64_C(0b0110110110110111011011011011011101101101101101110110110110110111),
        UINT64_C(0b0110111011011101101110110111011101101110110111011011101101110111),
        UINT64_C(0b0110111011101110110111011101110110111011101110110111011101110111),
        UINT64_C(0b0111011101110111011101110111011101110111011101110111011101110111),
        UINT64_C(0b0111011110111011110111011110111101110111101110111101110111101111),
        UINT64_C(0b0111011110111101111011110111101111011110111101111011110111101111),
        UINT64_C(0b0111101111011111011110111101111101111011110111110111101111011111),
        UINT64_C(0b0111110111110111111011111011111101111101111101111110111110111111),
        UINT64_C(0b0111111011111101111110111111011111101111110111111011111101111111),
        UINT64_C(0b0111111110111111110111111110111111110111111110111111110111111111),
        UINT64_C(0b0111111111011111111110111111111101111111110111111111101111111111),
        UINT64_C(0b0111111111111111011111111111111101111111111111110111111111111111),
        UINT64_C(0b0111111111111111111110111111111111111111110111111111111111111111),
        UINT64_C(0b0111111111111111111111111111111111111111111111111111111111111111)
};


ret_t LaserArray_Init(LaserArray_t *la, const LaserArray_Config_t *config) {
    // store the config
    la->config = *config;

    // create the semaphore lock
    const osSemaphoreAttr_t lock_attr = {
            .name = "laser_array_lock"
    };
    la->lock = osSemaphoreNew(1, 1, &lock_attr);
    RETURN_ON_FALSE(la->lock, RET_OUT_OF_MEMORY, "Failed to create lock");

    // clear the diode state and transfer data arrays
    memset(la->diodes, 0, sizeof(la->diodes));
    memset(la->tx_data, 0, sizeof(la->tx_data));

    // enable spi
    __HAL_SPI_ENABLE(la->config.hspi);

    // setup timer dma to trigger the transmission of spi data
    RETURN_ON_ERROR(
            HAL_DMA_Start_IT(la->config.htim_transfer->hdma[TIM_DMA_ID_UPDATE],
                (uint32_t) la->tx_data,
                (uint32_t) &la->config.hspi->Instance->DR,
                sizeof(la->tx_data) / sizeof(uint16_t)),
            "Failed to start dma stream");

    // enable dma on the timer
    __HAL_TIM_ENABLE_DMA(la->config.htim_transfer, TIM_DMA_UPDATE);

    // start the transfer timer with pwm enabled
    RETURN_ON_ERROR(HAL_TIM_PWM_Start(la->config.htim_transfer, la->config.rclk_channel),
            "Failed to start transfer timer in pwm mode");

    // start the fade update timer
    RETURN_ON_ERROR(HAL_TIM_Base_Start_IT(la->config.htim_fade),
            "Failed to start fade update timer");

    return RET_OK;
}

static void _LaserArray_SetBrightness(LaserArray_t *la, uint8_t diode_index, uint8_t brightness) {
    // clamp the brightness
    if (brightness >= LA_NUM_BRIGHTNESS_LEVELS) {
        brightness = LA_NUM_BRIGHTNESS_LEVELS - 1;
    }

    // return if the brightness did not change
    if (la->diodes[diode_index].current_brightness == brightness) {
        return;
    }

    // store the new brightness value
    la->diodes[diode_index].current_brightness = brightness;

    // lookup the brightness pattern
    la_brightness_pattern_t pattern = la_brightness_patterns[brightness];

    // iterate through each diode bitmask
    for (int i = 0; i < LA_TX_DATA_LENGTH; i++) {
        // either set or unset the corresponding bit depending on the pattern
        if ((pattern >> i) & UINT32_C(1)) {
            la->tx_data[i] |= UINT32_C(1) << diode_index;
        } else {
            la->tx_data[i] &= ~(UINT32_C(1) << diode_index);
        }
    }
}

ret_t LaserArray_SetBrightness(LaserArray_t *la, uint8_t diode_index, uint8_t brightness) {
    ret_t ret;
    osSemaphoreAcquire(la->lock, osWaitForever);

    // validate the diode index
    GOTO_EXIT_ON_FALSE(diode_index < LA_NUM_DIODES,
            RET_INVALID_ARG,
            "Invalid diode index: %u", diode_index);

    // call the internal update function
    _LaserArray_SetBrightness(la, diode_index, brightness);

    ret = RET_OK;
exit:
    osSemaphoreRelease(la->lock);
    return ret;
}

ret_t LaserArray_FadeBrightness(LaserArray_t *la, uint8_t diode_index, uint8_t brightness, uint32_t duration) {
    ret_t ret;
    osSemaphoreAcquire(la->lock, osWaitForever);

    // validate the diode index
    GOTO_EXIT_ON_FALSE(diode_index < LA_NUM_DIODES,
            RET_INVALID_ARG,
            "Invalid diode index: %u", diode_index);

    // setup the diode_index reference
    LaserArray_Diode_t *diode = &la->diodes[diode_index];
    diode->source_brightness = diode->current_brightness;
    diode->target_brightness = brightness;
    diode->transition_duration = duration * LA_FADE_TICK_RATE / 1000;
    diode->transition_tick = 0;

    // if duration is zero, set the brightness immediately
    if (diode->transition_duration == 0) {
        _LaserArray_SetBrightness(la, diode_index, brightness);
    }

    ret = RET_OK;
exit:
    osSemaphoreRelease(la->lock);
    return ret;
}

ret_t LaserArray_TIM_PeriodElapsedHandler(LaserArray_t *la, TIM_HandleTypeDef *htim) {
    // ignore callbacks addressed to other timers
    if (htim->Instance != la->config.htim_fade->Instance) {
        return RET_OK;
    }

    // update the fade for each diode
    for (uint8_t diode_index = 0; diode_index < LA_NUM_DIODES; diode_index++) {
        LaserArray_Diode_t *diode = &la->diodes[diode_index];

        // skip finished transitions
        if (diode->transition_tick == diode->transition_duration) {
            continue;
        }

        // calculate the new brightness
        int32_t source = diode->source_brightness;
        int32_t range = diode->target_brightness - diode->source_brightness;
        uint8_t brightness = source + range * (int32_t) (diode->transition_tick + 1) / (int32_t) diode->transition_duration;

        // set the new brightness value
        _LaserArray_SetBrightness(la, diode_index, brightness);

        // update the tick counter
        diode->transition_tick++;
    }

    return RET_OK;
}
