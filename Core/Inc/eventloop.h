#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "logging.h"


#define EVENT_MIDI_IN 0x00
#define EVENT_MIDI_OUT 0x10
#define EVENT_DIODE_STATE 0x20

#define EL_CAPACITY 128


typedef struct {
    uint8_t length; // must be the first byte within each event
    uint8_t type;
    uint8_t data[];
} __attribute__((__packed__)) Event_t;

typedef struct {
    uint8_t buffer[EL_CAPACITY];
    uint16_t size;
    uint16_t head;
    uint16_t tail;
} Eventloop_t;


ret_t EventLoop_Init(Eventloop_t *el);

ret_t EventLoop_Push(Eventloop_t *el, Event_t *event);
Event_t *EventLoop_Pop(Eventloop_t *el);
Event_t *EventLoop_Peek(Eventloop_t *el);

void EventLoop_Print(Eventloop_t *el);
