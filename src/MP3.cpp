#include "MP3.h"
#include "Arduino.h"

MP3::MP3() {
    this->offset = 0;
    this->received.bytes[0] = NOOP;
    this->initialized = false;
    this->ready = true;
    this->lastReceive = 0;
    this->lastSend = 0;
    this->timeout = 1000;
    this->stream = nullptr;
    this->feedbackEnabled = false;
    this->callback = nullptr;
}

uint8_t MP3::begin(Stream *stream, bool waitUntilInitialized, uint32_t timeout) {
    this->stream = stream;
    this->timeout = timeout;

    this->reset();

    if (waitUntilInitialized) {
        const unsigned long start = millis();

        //- Wait to hear back from the MP3 module on whether
        //- initialization was successful
        while (this->initialized == false) {
            this->loop();

            delay(0); //- watchdog friendly

            if (millis() - start > this->timeout)
                return MP3_ERROR_TIMEOUT;
        }

    } else {

#ifdef MP3_DEBUG
        SerialOut.println("Bypassing Initialization...");
#endif

        this->initialized = true;
    }

    return MP3_READY;
}

/**
 * The loop() method should be included in your main scratch's loop function. It ensures that
 * inbound messages and outbound messages are processed promptly.
 *
 * This function is non-blocking and instead keeps track of incoming bytes to form a comprehensible
 * inbound message.
 *
 * Please note:
 * When you call a command that interacts with the MP3 module, this class actually queues that
 * message. This ensures that we can ensure messages are only being sent when the MP3 module is
 * ready to receive them without having to rely on using pre-defined delay().
 */
void MP3::loop() {
    //- Process any inbound messages
    if (this->stream->available()) {
        uint8_t *data = this->received.bytes;

        data[this->offset] = this->stream->read();
        this->lastReceive = millis();

        //- Wait until we receive CMD_PREFIX before capturing additional message bytes
        if (data[0] == CMD_PREFIX) {
            this->offset += 1;
        }

        //- Once a full message has been received, process the full inbound message and reset
        //- all the message flags so we can start receiving the next message
        if (this->offset == COMMAND_PAYLOAD_SIZE) {
            this->processInboundMessage(this->received);
            this->received.bytes[0] = NOOP;
            this->offset = 0;
        }
    }

    //- Process the next message in the outbound queue if any are available
    if (this->cmdQueue.size() > 0 && this->ready) {
        Payload command = this->cmdQueue.pop();

#ifdef MP3_DEBUG
        SerialOut.print("Sending Message: ");
        printCommand(command);
        SerialOut.println();
#endif

        this->stream->write(command.bytes, COMMAND_PAYLOAD_SIZE);
        this->lastSend = millis();
    }
}

void MP3::processInboundMessage(const Payload &payload) {
    const bool validMessage = validateMessage(payload);

    if (validMessage) {
        if (payload.CMD == RESPONSE_ACK) {
            this->ready = true;
        }

        if (payload.CMD == RESPONSE_ONLINE) {
            this->ready = true;
            this->initialized = true;
        }

#ifdef MP3_DEBUG
        SerialOut.print("Received Message: ");
        printCommand(payload);
        SerialOut.println();
#endif

        if (this->callback != nullptr) {
            this->callback(payload.CMD, VAL(payload.DATA_HI, payload.DATA_LO));
        }
    }
}

// 7E FF 06 0C | 00 00 00 EF
void MP3::reset() { this->queueCommand(CMD_OP_RESET); }

// 7E FF 06 01 | 00 00 00 EF
void MP3::next() { this->queueCommand(CMD_OP_TRACK_NEXT); }

// 7E FF 06 02 | 00 00 00 EF
void MP3::prev() { this->queueCommand(CMD_OP_TRACK_PREV); }

// 7E FF 06 06 | 00 00 XX EF
void MP3::volume(uint8_t vol) { this->queueCommand(CMD_OP_VOLUME_SET, HI(vol), LO(vol)); }

// 7E FF 06 0D | 00 00 00 EF
void MP3::play() { this->queueCommand(CMD_OP_PLAY); }

// 7E FF 06 03 | 00 00 00 EF
void MP3::playTrack(uint8_t track) { this->queueCommand(CMD_OP_PLAY_TRACK, HI(track), LO(track)); }

// 7E FF 06 0E | 00 00 00 EF
void MP3::pause() { this->queueCommand(CMD_OP_PAUSE); }

// 7E FF 06 16 | 00 00 00 EF
void MP3::stop() { this->queueCommand(CMD_OP_STOP); }

// 7E FF 06 07 | 00 00 XX EF
void MP3::eq(uint8_t mode) { this->queueCommand(CMD_OP_EQ, HI(mode), LO(mode)); }

// 7E FF 06 1A | 00 00 00 EF
void MP3::dac(uint8_t onOff) { this->queueCommand(CMD_OP_DAC, HI(onOff), LO(onOff)); }

void MP3::onMessage(messageReceivedCallback cb) { this->callback = cb; }

void MP3::queueCommand(uint8_t cmd, uint8_t hdata, uint8_t ldata) {
    Payload command;

    command.HEAD = CMD_PREFIX;
    command.VERSION = CMD_VERSION;
    command.LENGTH = CMD_LENGTH;
    command.CMD = cmd;
    command.FEEDBACK = this->feedbackEnabled ? 0x01 : 0x00;
    command.DATA_HI = hdata;
    command.DATA_LO = ldata;
    command.CHKSUM_HI = 0x00;
    command.CHKSUM_LO = 0x00;
    command.END = CMD_END;

    calculateChecksum(command);

    this->cmdQueue.add(0, command);
}


