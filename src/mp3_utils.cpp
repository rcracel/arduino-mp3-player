//
// Created by Roger Cracel on 11/16/24.
//

#include "mp3_utils.h"

void printCommand(Payload &command) {
    const uint8_t *values = command.bytes;

    uint16_t eq;
    for (int i = 0; i < COMMAND_PAYLOAD_SIZE; i++) {
        uint8_t value = values[i];
        SerialOut.print(value, HEX);
        SerialOut.print(" ");
    }
    switch (command.CMD) {
        case CMD_OP_RESET:
            SerialOut.print(" (RESET)");
            break;
        case CMD_OP_TRACK_NEXT:
            SerialOut.print(" (NEXT TRACK)");
            break;
        case CMD_OP_TRACK_PREV:
            SerialOut.print(" (PREV TRACK)");
            break;
        case CMD_OP_VOLUME_SET:
            SerialOut.print(" (SET VOLUME ");
            SerialOut.print(VAL(command.DATA_HI, command.DATA_LO));
            SerialOut.print(")");
            break;
        case CMD_OP_PLAY:
            SerialOut.print(" (PLAY)");
            break;
        case CMD_OP_PLAY_TRACK:
            SerialOut.print(" (PLAY TRACK ");
            SerialOut.print(VAL(command.DATA_HI, command.DATA_LO));
            SerialOut.print(")");
            break;
        case CMD_OP_STOP:
            SerialOut.print(" (STOP)");
            break;
        case CMD_OP_PAUSE:
            SerialOut.print(" (PAUSE)");
            break;
        case CMD_OP_DAC:
            SerialOut.print(" (SET DAC ");
            SerialOut.print(VAL(command.DATA_HI, command.DATA_LO) == 0 ? "OFF" : "ON");
            SerialOut.print(")");
            break;
        case CMD_OP_EQ:
            SerialOut.print(" (SET EQ ");
            eq = VAL(command.DATA_HI, command.DATA_LO);
            switch (eq) {
                case EQ_NORMAL:
                    SerialOut.print("EQ_NORMAL");
                    break;
                case EQ_ROCK:
                    SerialOut.print("EQ_ROCK");
                    break;
                case EQ_POP:
                    SerialOut.print("EQ_POP");
                    break;
                case EQ_CLASSIC:
                    SerialOut.print("EQ_CLASSIC");
                    break;
                case EQ_COUNTRY:
                    SerialOut.print("EQ_COUNTRY");
                    break;
                case EQ_JAZZ:
                    SerialOut.print("EQ_JAZZ");
                    break;
                default:
                    SerialOut.print("UNKNOWN");
            }
            SerialOut.print(")");
            break;
        default:
            SerialOut.print("(UNKNOWN)");
            break;
    }
    SerialOut.println();
}
bool validateMessage(const Payload &payload) {

    if (payload.HEAD != CMD_PREFIX) {
#ifdef MP3_DEBUG
        SerialOut.print("Invalid message header: ");
        SerialOut.print(payload.HEAD);
        SerialOut.println();
#endif
        return false;
    }

    if (payload.VERSION != CMD_VERSION) {
#ifdef MP3_DEBUG
        SerialOut.print("Invalid message version: ");
        SerialOut.print(payload.VERSION);
        SerialOut.println();
#endif
    }

    if (payload.LENGTH != CMD_LENGTH) {
#ifdef MP3_DEBUG
        SerialOut.print("Invalid message length: ");
        SerialOut.print(payload.LENGTH);
        SerialOut.println();
#endif
        return false;
    }

    if (payload.END != CMD_END) {
        SerialOut.print("Invalid message end: ");
        SerialOut.print(payload.END);
        SerialOut.println();
        return false;
    }

    //- TODO CHECKSUM VALIDATION

    return true;
}
void calculateChecksum(Payload &payload) {
    int16_t checksum =
            0 - payload.VERSION - payload.LENGTH - payload.CMD - payload.FEEDBACK - payload.DATA_HI - payload.DATA_LO;

    payload.CHKSUM_HI = HI(checksum);
    payload.CHKSUM_LO = LO(checksum);
}

