#pragma once


#include "stm32f1xx_hal.h"
#ifdef DEBUG
    #include "stdio.h"
#endif


#ifndef unlikely
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif


#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4


#define LOG_LEVEL_STR(level) (((const char *[]) { "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL" })[level])

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

#define LOG_ERROR_CODE(error_code, message, ...) LOG_ERROR(message " (code 0x%02x)" __VA_OPT__(,) __VA_ARGS__, error_code)


#define RETURN_ON_ERROR(x, message, ...) do { \
    HAL_StatusTypeDef _ret = (x); \
    if (unlikely(_ret != HAL_OK)) { \
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
    HAL_StatusTypeDef _ret = (x); \
    if (unlikely(_ret != HAL_OK)) { \
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
    HAL_StatusTypeDef _ret = (x); \
    if (unlikely(_ret != HAL_OK)) { \
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
