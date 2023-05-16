#include "eventloop.h"
#include "string.h"
#include "stddef.h"


ret_t EventLoop_Init(Eventloop_t *el) {
    memset(el->buffer, 0, EL_CAPACITY);
    el->tail = 0;
    el->head = 0;
    el->size = 0;

    return RET_OK;
}

ret_t EventLoop_Push(Eventloop_t *el, Event_t *event) {
    // check the required size
    uint16_t length = event->length + 2;

    // validate the resulting tail position
    uint16_t new_tail = el->tail + length;
    if (new_tail <= EL_CAPACITY) {
        if (el->head > el->tail && new_tail > el->head) {
            return RET_QUEUE_FULL;
        }

        // append to the tail
        memcpy(&el->buffer[el->tail], event, length);
    } else {
        new_tail = length;

        if (el->tail > new_tail && new_tail > el->head) {
            return RET_QUEUE_FULL;
        }

        // store only the length byte at the current position
        el->buffer[el->tail] = event->length;

        // append to the start of the buffer
        memcpy(el->buffer, event, length);
    }

    // adjust the tail and size
    el->tail = new_tail % EL_CAPACITY;
    el->size++;

    return RET_OK;
}

Event_t *EventLoop_Pop(Eventloop_t *el) {
    // check size
    if (el->size == 0) {
        return NULL;
    }

    // get the length of the head event
    uint16_t length = el->buffer[el->head] + 2;

    // check if the event wraps around and adjust the new head position
    Event_t *event;
    if (el->head + length <= EL_CAPACITY) {
        event = (Event_t *) &el->buffer[el->head];
        el->head = (el->head + length) % EL_CAPACITY;
    } else {
        event = (Event_t *) el->buffer;
        el->head = length;
    }

    // adjust the size
    el->size--;

    return event;
}

Event_t *EventLoop_Peek(Eventloop_t *el) {
    // check size
    if (el->size == 0) {
        return NULL;
    }

    // get the length of the head event
    uint16_t length = el->buffer[el->head] + 2;

    // return the event at either the head position or the start of the buffer, depending on if it wraps around
    if (el->head + length <= EL_CAPACITY) {
        return (Event_t *) &el->buffer[el->head];
    } else {
        return (Event_t *) el->buffer;
    }
}


void EventLoop_Print(Eventloop_t *el) {
    uint16_t x = el->head;

    printf("Eventloop (%u events):\n", el->size);
    for (int i = 0; i < el->size; i++) {
        uint16_t length = el->buffer[x] + 2;
        if (x + length > EL_CAPACITY) x = 0;

        Event_t *event = (Event_t *) &el->buffer[x];
        printf("   Event(T=0x%02x, L=0x%02x): ", event->type, event->length);
        for (int j = 0; j < event->length; j++) {
            printf("%02x ", event->data[j]);
        }
        printf("\n");
    }
}
