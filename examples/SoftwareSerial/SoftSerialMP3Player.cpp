
#include <SoftwareSerial.h>
#include <MP3.h>

#define BAUD_RATE 9600

#define SERIAL_RX 4
#define SERIAL_TX 5

SoftwareSerial mp3Serial(SERIAL_RX, SERIAL_TX);
MP3 mp3Player;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(0); }

  mp3Serial.begin(9600);

  Serial.println("Initializing MP3 Player Interface");
  const uint8_t initialization = mp3Player.begin(&mp3Serial, true, 1000);
  if (initialization == MP3_READY) {
    Serial.println("MP3 player initialized!");
  } else if (initialization == MP3_ERROR_TIMEOUT) {
    Serial.println("MP3 player timeout!");
  } else {
    Serial.println("MP3 player FAILED!");
  }

  mp3Player.volume(25);
  mp3Player.next();
}

void loop() {

}