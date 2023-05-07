#pragma once


#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90


typedef struct {
    uint8_t command;
    uint8_t data[];
} midi_messate_t;

typedef struct {
    uint8_t command;
    uint8_t data[2];
} midi_short_message_t;
