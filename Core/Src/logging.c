#include "logging.h"


extern UART_HandleTypeDef *__log_uart;


const char *__log_level_str[] = {
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL"
};

const char *__log_ret_str[] = {
        "Ok",
        "Error",
        "Busy",
        "Timeout",
        "Out of memory",
        "Invalid argument",
        "Invalid state",
        "Invalid size",
        "Not found",
        "Not supported",
        "Invalid response",
        "Invalid crc",
        "Invalid verion",
        "Queue empty",
        "Queue full"
};

PUTCHAR_PROTOTYPE {
    if (ch == '\n') {
        uint8_t cr = '\r';
        HAL_UART_Transmit(__log_uart, &cr, 1, HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(__log_uart, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    return ch;
}
