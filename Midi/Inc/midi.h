#pragma once


#include "dinmidi.h"
#include "usbmidi.h"


typedef struct {
    DinMidi_Config_t din;
    UsbMidi_Config_t usb;
} Midi_Config_t;


typedef struct {
    Midi_Config_t config;

    UsbMidi_t usb;
    DinMidi_t din;
} Midi_t;


ret_t Midi_ReceiveCallback(Midi_t *midi, uint8_t *message, uint16_t length);

ret_t Midi_Init(Midi_t *midi, const Midi_Config_t *config);
ret_t Midi_Transmit(Midi_t *midi, uint8_t *message, uint16_t length);
