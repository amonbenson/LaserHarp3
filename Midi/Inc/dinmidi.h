#pragma once


#include "logging.h"
#include "stm32f1xx_hal.h"


#define DINMIDI_RX_BUFFER_SIZE 64


typedef struct {
    UART_HandleTypeDef *huart;
} DinMidi_Config_t;

typedef struct {
    DinMidi_Config_t config;

    uint8_t rx_buffer[DINMIDI_RX_BUFFER_SIZE];
} DinMidi_t;


ret_t DinMidi_ReceiveCallback(DinMidi_t *dinmidi, uint8_t *data, uint16_t length);

ret_t DinMidi_Init(DinMidi_t *dinmidi, const DinMidi_Config_t *config);
ret_t DinMidi_Transmit(DinMidi_t *dinmidi, uint8_t *data, uint16_t length);

ret_t DinMidi_UARTEx_RxEventHandler(DinMidi_t *dinmidi, UART_HandleTypeDef *huart, uint16_t Size);
