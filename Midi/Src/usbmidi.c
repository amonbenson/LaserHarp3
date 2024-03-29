#include "usbmidi.h"
#include "usbd_midi.h"
#include "usbd_def.h"
#include "stm32f1xx_hal.h"


__weak ret_t UsbMidi_ReceiveCallback(UsbMidi_t *usbmidi, uint8_t *data, uint16_t length) {
    return RET_OK;
}


ret_t UsbMidi_Init(UsbMidi_t *usbmidi, const UsbMidi_Config_t *config) {
    // store the config
    usbmidi->config = *config;

    return RET_OK;
}

static ret_t UsbMidi_CreatePacket(UsbMidi_Packet_t *packet, uint8_t cn, uint8_t *data, uint16_t length) {
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

            // validate the length and store the data
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

    // if usb is not connected, ignore the packet transmission
    if (!UsbMidi_IsConnected(usbmidi)) {
        LOG_WARN("Cannot transmit: USB not connected.");
        return RET_OK;
    }

    // wait for idle state and send the packet
    USBD_MIDI_SendReport(usbmidi->config.hUsbDeviceFS, (uint8_t *) &packet, sizeof(UsbMidi_Packet_t));
    while (USBD_MIDI_GetState(usbmidi->config.hUsbDeviceFS) != MIDI_IDLE) {}

    return RET_OK;
}

bool UsbMidi_IsConnected(UsbMidi_t *usbmidi) {
    return usbmidi->config.hUsbDeviceFS->dev_state == USBD_STATE_CONFIGURED;
}

ret_t UsbMidi_DataInHandler(UsbMidi_t *usbmidi, uint8_t *usb_rx_buffer, uint8_t usb_rx_buffer_length) {
    while (usb_rx_buffer_length > 0 && *usb_rx_buffer != 0x00) {
        // get the cn and cin numbers
        //uint8_t cn = usb_rx_buffer[0] >> 4;
        //uint8_t cin = usb_rx_buffer[0] & 0x0F;

        // invoke the callback
        RETURN_ON_ERROR(UsbMidi_ReceiveCallback(usbmidi, &usb_rx_buffer[1], 3), "RX Callback failed");

        // select the next packet
        usb_rx_buffer += 4;
        usb_rx_buffer_length -= 4;
    }

    return RET_OK;
}
