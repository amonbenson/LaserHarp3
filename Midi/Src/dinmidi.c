#include "dinmidi.h"
#include "logging.h"
#include "string.h"


__weak ret_t DinMidi_ReceiveCallback(DinMidi_t *dinmidi, uint8_t *message, uint16_t length) {
    return RET_OK;
}

ret_t DinMidi_Init(DinMidi_t *dinmidi, const DinMidi_Config_t *config) {
    // store the config
    dinmidi->config = *config;

    // clear the rx buffer
    memset(dinmidi->rx_buffer, 0, sizeof(dinmidi->rx_buffer));

    // start receiving data
    RETURN_ON_ERROR(HAL_UARTEx_ReceiveToIdle_IT(dinmidi->config.huart, dinmidi->rx_buffer, sizeof(dinmidi->rx_buffer)), "Failed to receive.");
    __HAL_UART_ENABLE_IT(dinmidi->config.huart, UART_IT_IDLE);

    return RET_OK;
}

ret_t DinMidi_Transmit(DinMidi_t *dinmidi, uint8_t *data, uint16_t length) {
    // transmit
    RETURN_ON_ERROR(HAL_UART_Transmit(
            dinmidi->config.huart,
            data,
            length,
            HAL_MAX_DELAY),
            "Uart transmission failed.");

    return RET_OK;
}

ret_t DinMidi_UARTEx_RxEventHandler(DinMidi_t *dinmidi, UART_HandleTypeDef *huart, uint16_t Size) {
    // ignore callbacks addressed to other uarts
    if (huart->Instance != dinmidi->config.huart->Instance) {
        return HAL_OK;
    }

    // invoke the callback
    RETURN_ON_ERROR(DinMidi_ReceiveCallback(dinmidi, dinmidi->rx_buffer, Size), "Failed to invoke callback");

    // receive the next packet
    RETURN_ON_ERROR(HAL_UARTEx_ReceiveToIdle_IT(dinmidi->config.huart, dinmidi->rx_buffer, sizeof(dinmidi->rx_buffer)), "Failed to receive.");

    return RET_OK;
}
