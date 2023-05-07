/**
  ******************************************************************************
  * @file    usbd_hid.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_hid_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MIDI_H
#define __USB_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for usbd_hid.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */
#define MIDI_EPIN_ADDR                 0x81U
#define MIDI_EPIN_SIZE                 0x04U

#define USB_MIDI_CONFIG_DESC_SIZ       34U
#define USB_MIDI_DESC_SIZ              9U
#define USB_MIDI_MOUSE_REPORT_DESC_SIZE    74U

#define MIDI_DESCRIPTOR_TYPE           0x21U
#define MIDI_REPORT_DESC               0x22U

#ifndef MIDI_HS_BINTERVAL
#define MIDI_HS_BINTERVAL            0x07U
#endif /* MIDI_HS_BINTERVAL */

#ifndef MIDI_FS_BINTERVAL
#define MIDI_FS_BINTERVAL            0x0AU
#endif /* HID_FS_BINTERVAL */

#define MIDI_REQ_SET_PROTOCOL          0x0BU
#define MIDI_REQ_GET_PROTOCOL          0x03U

#define MIDI_REQ_SET_IDLE              0x0AU
#define MIDI_REQ_GET_IDLE              0x02U

#define MIDI_REQ_SET_REPORT            0x09U
#define MIDI_REQ_GET_REPORT            0x01U
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
  USBD_MIDI_IDLE = 0,
  USBD_MIDI_BUSY,
}
USBD_MIDI_StateTypeDef;


typedef struct
{
  uint32_t             Protocol;
  uint32_t             IdleState;
  uint32_t             AltSetting;
  USBD_MIDI_StateTypeDef     state;
}
USBD_MIDI_HandleTypeDef;
/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_MIDI;
#define USBD_MIDI_CLASS    &USBD_MIDI
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_MIDI_SendReport(USBD_HandleTypeDef *pdev,
                            uint8_t *report,
                            uint16_t len);

uint32_t USBD_MIDI_GetPollingInterval(USBD_HandleTypeDef *pdev);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_HID_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
