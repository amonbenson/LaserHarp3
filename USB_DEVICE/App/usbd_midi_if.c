/*
 *  11/26/17 by C.P.: Version 0.8.0 - Universal Version
 */

#include "usbd_midi_if.h"
#include "stm32f1xx_hal.h"

uint8_t SysexID[15] = {
        0xF0,
        0x7E,
        0x00,
        0x06,
        0x02,
        0x7D,
        0x01,
        0x02,
        0x03,
        0x04,
        0x00,
        0x00,
        0x00,
        0x00,
        0xF7
};

USBD_MIDI_ItfTypeDef USBD_Interface_fops_FS =
{
  MIDI_DataRx,
  MIDI_DataTx
};

__weak uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length)
{
  return 0;
}

uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length)
{
  uint32_t i = 0;
  while (i < length) {
    APP_Rx_Buffer[APP_Rx_ptr_in] = *(msg + i);
    APP_Rx_ptr_in++;
    i++;
    if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) {
      APP_Rx_ptr_in = 0;
    }
  }
  USBD_MIDI_SendPacket();
  return USBD_OK;
}
