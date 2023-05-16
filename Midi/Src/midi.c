#include "midi.h"

#include "stddef.h"


__weak ret_t Midi_ReceiveCallback(Midi_t *midi, uint8_t *message, uint16_t length) {
    return RET_OK;
}


ret_t Midi_Init(Midi_t *midi, const Midi_Config_t *config) {
    // init usbmidi
    HALT_ON_ERROR(UsbMidi_Init(&midi->usb, &config->usb),
        "Failed to initialize usbmidi.");

    // init dinmidi
    HALT_ON_ERROR(DinMidi_Init(&midi->din, &config->din),
        "Failed to initialize dinmidi.");

    return RET_OK;
}

ret_t Midi_Transmit(Midi_t *midi, uint8_t *message, uint16_t length) {
    // transmit via din
    RETURN_ON_ERROR(DinMidi_Transmit(&midi->din, message, length), "Failed to transmit din midi");

    // transmit via usb if available
    if (UsbMidi_IsConnected(&midi->usb)) {
        RETURN_ON_ERROR(UsbMidi_Transmit(&midi->usb, message, length), "Failed to transmit usb midi");
    }

    return RET_OK;
}


// lower level callback implementations

ret_t UsbMidi_ReceiveCallback(UsbMidi_t *usbmidi, uint8_t *message, uint16_t length) {
    Midi_t *midi = (Midi_t *) (usbmidi - offsetof(Midi_t, usb));

    return Midi_ReceiveCallback(midi, message, length);
}

ret_t DinMidi_ReceiveCallback(DinMidi_t *dinmidi, uint8_t *message, uint16_t length) {
    Midi_t *midi = (Midi_t *) (dinmidi - offsetof(Midi_t, din));

    return Midi_ReceiveCallback(midi, message, length);
}
