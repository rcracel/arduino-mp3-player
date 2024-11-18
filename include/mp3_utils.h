//
// Created by Roger Cracel on 11/16/24.
//

#ifndef MP3_UTILS_H
#define MP3_UTILS_H

#include <Arduino.h>
#include "mp3_settings.h"

#ifndef SerialOut
#define SerialOut Serial
#endif

typedef void (*messageReceivedCallback)(uint8_t, uint32_t);

typedef union {
    uint8_t bytes[COMMAND_PAYLOAD_SIZE];
    struct {
        uint8_t HEAD;
        uint8_t VERSION;
        uint8_t LENGTH;
        uint8_t CMD;
        uint8_t FEEDBACK;
        uint8_t DATA_HI;
        uint8_t DATA_LO;
        uint8_t CHKSUM_HI;
        uint8_t CHKSUM_LO;
        uint8_t END;
    };
} Payload;

void printCommand(Payload &command);
bool validateMessage(const Payload &payload);
void calculateChecksum(Payload &payload);

#endif //MP3_UTILS_H
