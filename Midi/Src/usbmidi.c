#include "usbmidi.h"


ret_t UsbMidi_Init(UsbMidi_t *usbmidi, const UsbMidi_Config_t *config) {
    // store the config
    usbmidi->config = *config;

    return RET_OK;
}

ret_t UsbMidi_CreatePacket(UsbMidi_Packet_t *packet, uint8_t cn, uint8_t *data, uint16_t length) {
    RETURN_ON_FALSE(length > 0, RET_INVALID_SIZE, "Cannot create zero length packet.");

    uint8_t command = data[0] & 0xF0;

    // set the packet channel number
    packet->cn_cin = cn << 4;

    // decode the midi data into a packet
    switch (command) {
        case MIDI_NOTE_OFF:
        case MIDI_NOTE_ON:
            // set the cin
            packet->cn_cin |= command >> 4;

            // validate the length
            RETURN_ON_FALSE(length >= 3, RET_INVALID_SIZE, "Remaining data must be at least 3 bytes long.");
            memcpy(&packet->data, data, 3);
            break;
        default:
            LOG_ERROR("Unsupported midi command: 0x%02x.", command);
    }

    return RET_OK;
}

ret_t UsbMidi_Transmit(UsbMidi_t *usbmidi, uint8_t *data, uint16_t length) {
    UsbMidi_Packet_t packet;

    // create the midi packet
    RETURN_ON_ERROR(UsbMidi_CreatePacket(&packet, 0, data, length), "Failed to construct usbmidi packet.");

    // send the packet
    USBD_MIDI_SendReport(usbmidi->config.hUsbDeviceFS, (uint8_t *) &packet, sizeof(UsbMidi_Packet_t));

    // busy-wait until the packet got transmitted
    while (((USBD_MIDI_HandleTypeDef *) usbmidi->config.hUsbDeviceFS->pClassData)->state == USBD_MIDI_BUSY) {}

    return RET_OK;
}
