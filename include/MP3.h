#pragma once

#ifndef MP3_CONTROLLER_H
#define MP3_CONTROLLER_H

#include <LinkedList.h>
#include <Stream.h>

#include "mp3_utils.h"

#define MP3_DEBUG

#ifndef SerialOut
#define SerialOut Serial
#endif

#define MP3_READY         0x00
#define MP3_ERROR_TIMEOUT 0x66

class MP3 {
private:
  Stream *stream;

  LinkedList<Payload> cmdQueue = LinkedList<Payload>();
  uint32_t _timeout;

  uint32_t lastSend;
  uint32_t lastReceive;

  bool feedbackEnabled;
  bool initialized;
  bool ready;
  
  Payload received{};
  uint8_t offset;

  messageReceivedCallback callback;

  void processInboundMessage(Payload &payload);

protected:
  void queueCommand(uint8_t cmd, uint8_t hdata = 0x00, uint8_t ldata = 0x00);

public:
  MP3();

  uint8_t begin(Stream *stream, bool waitUntilInitialized = true, uint32_t timeout = 2000);
  void loop();

  void reset();

  void play();
  void playTrack(uint8_t track);

  void pause();
  void stop();
  void eq(uint8_t mode);
  void dac(uint8_t onOff);
  void next();
  void prev();
  void volume(uint8_t vol);

  void onMessage(messageReceivedCallback cb);
};

#endif