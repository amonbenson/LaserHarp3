#pragma once


extern const char *__log_ret_str[];


#if !defined(LOG_PRINTF) && defined(DEBUG)
    #include "stdio.h"
    #define LOG_PRINTF(fmt, ...) printf(fmt, ## __VA_ARGS__)
#endif

#ifndef LOG_RETURN_TYPE
    typedef enum {
        RET_OK = 0x00U,
        RET_ERROR = 0x01U,
        RET_BUSY = 0x02U,
        RET_TIMEOUT = 0x03U,
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

    // set the return type and success code variables
    #define LOG_RETURN_TYPE ret_t
    #define __LOG_RETURN_TYPE_STRING(ret) (((const char *[]) { \
        "Ok", \
        "Error", \
        "Busy", \
        "Timeout", \
        "Out of memory", \
        "Invalid argument", \
        "Invalid state", \
        "Invalid size", \
        "Not found", \
        "Not supported", \
        "Invalid response", \
        "Invalid crc", \
        "Invalid verion", \
        "Queue empty", \
        "Queue full" \
    })[ret])
    #define __LOG_RETURN_TYPE_CLAMP(ret) (((ret) < 0 || (ret) >= __RET_LENGTH) ? RET_ERROR : (ret))
    #define LOG_RETURN_TYPE_STRING(ret) __LOG_RETURN_TYPE_STRING(__LOG_RETURN_TYPE_CLAMP(ret))
    #define LOG_SUCCESS_CODE RET_SUCCESS
#endif

#ifndef LOG_SUCCESS_CODE
    #error "Please set the macro LOG_SUCCESS_CODE manually before including logging.h"
#endif

#ifndef LOG_RETURN_TYPE_STRING
    #error "Please set the macro LOG_RETURN_TYPE_STRING(ret) manually before including logging.h"
#endif

#ifndef LOG_RETURN_VARIABLE
    #define LOG_RETURN_VARIABLE ret
#endif

#define LOG_HALT_PROCEDURE() do { \
    __disable_irq(); \
    while (1) {}; \
} while (0)


#ifndef unlikely
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif


#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

#define __LOG_LEVEL_STRING(level) (((const char *[]) { \
    "DEBUG", \
    "INFO ", \
    "WARN ", \
    "ERROR", \
    "FATAL" \
})[level])

//#define __LOG_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __LOG_FILENAME __FILE__

#ifdef DEBUG
    #define __LOG_MESSAGE(level, message, ...) LOG_PRINTF("%s %s:%d: " message "\n", __LOG_LEVEL_STRING(level), __LOG_FILENAME, __LINE__, ## __VA_ARGS__)
#else
    #define __LOG_MESSAGE(level, message, ...) ((void) 0)
#endif


#define LOG_DEBUG(message, ...) __LOG_MESSAGE(LOG_LEVEL_DEBUG, message, ## __VA_ARGS__)
#define LOG_INFO(message, ...) __LOG_MESSAGE(LOG_LEVEL_INFO, message, ## __VA_ARGS__)
#define LOG_WARN(message, ...) __LOG_MESSAGE(LOG_LEVEL_WARN, message, ## __VA_ARGS__)
#define LOG_ERROR(message, ...) __LOG_MESSAGE(LOG_LEVEL_ERROR, message, ## __VA_ARGS__)
#define LOG_FATAL(message, ...) __LOG_MESSAGE(LOG_LEVEL_FATAL, message, ## __VA_ARGS__)

#define __LOG_ERROR_CODE(error_code, message, ...) LOG_ERROR(message " (%s)", ## __VA_ARGS__, LOG_RETURN_TYPE_STRING(error_code))


#define RETURN_ON_ERROR(x, message, ...) do { \
    LOG_RETURN_TYPE _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        __LOG_ERROR_CODE(_ret, message, ## __VA_ARGS__); \
        return _ret; \
    } \
} while (0)

#define RETURN_ON_FALSE(expression, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        __LOG_ERROR_CODE((error_code), message, ## __VA_ARGS__); \
        return (error_code); \
    } \
} while (0)

#define GOTO_EXIT_ON_ERROR(x, message, ...) do { \
    LOG_RETURN_TYPE _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        __LOG_ERROR_CODE(_ret, message, ## __VA_ARGS__); \
        LOG_RETURN_VARIABLE = _ret; \
        goto exit; \
    } \
} while (0)

#define GOTO_EXIT_ON_FALSE(expression, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        __LOG_ERROR_CODE((error_code), message, ## __VA_ARGS__); \
        LOG_RETURN_VARIABLE = (error_code); \
        goto exit; \
    } \
} while (0)

#define HALT_ON_ERROR(x, message, ...) do { \
    LOG_RETURN_TYPE _ret = (x); \
    if (unlikely(_ret != RET_OK)) { \
        __LOG_ERROR_CODE(_ret, message, ## __VA_ARGS__); \
        LOG_HALT_PROCEDURE(); \
    } \
} while (0)

#define HALT_ON_FALSE(x, error_code, message, ...) do { \
    if (unlikely(!(expression))) { \
        __LOG_ERROR_CODE((error_code), message, ## __VA_ARGS__); \
        LOG_HALT_PROCEDURE(); \
    } \
} while (0)
