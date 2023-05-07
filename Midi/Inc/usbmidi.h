#pragma once


#include "stdbool.h"
#include "usb_device.h"
#include "midi_types.h"
#include "logging.h"


typedef struct {
    USBD_HandleTypeDef *hUsbDeviceFS;
} UsbMidi_Config_t;

typedef struct {
    uint8_t cn_cin;
    uint8_t data[3];
} __packed UsbMidi_Packet_t;;

typedef struct {
    UsbMidi_Config_t config;
} UsbMidi_t;


ret_t UsbMidi_Init(UsbMidi_t *usbmidi, const UsbMidi_Config_t *config);

ret_t UsbMidi_Transmit(UsbMidi_t *usbmidi, uint8_t *data, uint16_t length);
bool UsbMidi_IsConnected(UsbMidi_t *usbmidi);
