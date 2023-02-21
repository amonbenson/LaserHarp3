#pragma once


#include "stm32f1xx_hal.h"
#ifdef DEBUG
    #include "stdio.h"
#endif


#ifndef unlikely
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


extern UART_HandleTypeDef *__log_uart;
extern const char *__log_level_str[];
extern const char *__log_ret_str[];

typedef enum {
    RET_OK = 0x00U,
    RET_ERROR = HAL_ERROR,
    RET_BUSY = HAL_BUSY,
    RET_TIMEOUT = HAL_TIMEOUT,
    RET_OUT_OF_MEMORY = 0x04U,
    RET_INVALID_ARG = 0x05U,
    RET_INVALID_STATE = 0x06U,
    RET_INVALID_SIZE = 0x07U,
    RET_NOT_FOUND = 0x08U,
    RET_NOT_SUPPORTED = 0x09U,
    RET_INVALID_RESPONSE = 0x10U,
    RET_INVALID_CRC = 0x1aU,
    RET_INVALID_VERSION = 0x1bU,
    RET_QUEUE_EMPTY = 0x1dU,
    RET_QUEUE_FULL = 0x1cU,
    __RET_LENGTH
} ret_t;


#define LOG_SET_UART(huart) UART_HandleTypeDef *__log_uart = (huart)


#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

#define LOG_LEVEL_STR(level) (__log_level_str[level])
#define RET_STR(ret) (__log_ret_str[(ret) >= __RET_LENGTH ? 0x01U : (ret)])

#ifdef DEBUG
    #define LOG_MESSAGE(level, message, ...) printf("%s %s:%d: " message "\n", LOG_LEVEL_STR(level), __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#else
    #define LOG_MESSAGE(level, message, ...) ((void) 0)
#endif


#define LOG_DEBUG(message, ...) LOG_MESSAGE(LOG_LEVEL_DEBUG, message __VA_OPT__(,) __VA_ARGS__)
#define LOG_INFO(message, ...) LOG_MESSAGE(LOG_LEVEL_INFO, message __VA_OPT__(,) __VA_ARGS__)
#define LOG_WARN(message, ...) LOG_MESSAGE(LOG_LEVEL_WARN, message __VA_OPT__(,) __VA_ARGS__)
#define LOG_ERROR(message, ...) LOG_MESSAGE(LOG_LEVEL_ERROR, message __VA_OPT__(,) __VA_ARGS__)
#define LOG_FATAL(message, ...) LOG_MESSAGE(LOG_LEVEL_FATAL, message __VA_OPT__(,) __VA_ARGS__)

#define LOG_ERROR_CODE(error_code, message, ...) LOG_ERROR(message " (%s)" __VA_OPT__(,) __VA_ARGS__, RET_STR(error_code))


#define RETURN_ON_ERROR(x, message, ...) do { \
    ret_t _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        LOG_ERROR_CODE(_ret, message __VA_OPT__(,) __VA_ARGS__); \
        return _ret; \
    } \
} while (0)

#define RETURN_ON_FALSE(expression, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        LOG_ERROR_CODE((error_code), message __VA_OPT__(,) __VA_ARGS__); \
        return (error_code); \
    } \
} while (0)

#define GOTO_EXIT_ON_ERROR(x, message, ...) do { \
    ret_t _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        LOG_ERROR_CODE(_ret, message __VA_OPT__(,) __VA_ARGS__); \
        ret = _ret; \
        goto exit; \
    } \
} while (0)

#define GOTO_EXIT_ON_FALSE(expression, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        LOG_ERROR_CODE((error_code), message __VA_OPT__(,) __VA_ARGS__); \
        ret = (error_code); \
        goto exit; \
    } \
} while (0)

#define HALT_ON_ERROR(x, message, ...) do { \
    ret_t _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        LOG_ERROR_CODE(_ret, message __VA_OPT__(,) __VA_ARGS__); \
        __disable_irq(); \
        while (1) {}; \
    } \
} while (0)

#define HALT_ON_FALSE(x, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        LOG_ERROR_CODE((error_code), message __VA_OPT__(,) __VA_ARGS__); \
        __disable_irq(); \
        while (1) {}; \
    } \
} while (0)
