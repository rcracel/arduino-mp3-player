#ifndef MP3_SETTINGS_H
#define MP3_SETTINGS_H

#define COMMAND_FEEDBACK_TIMEOUT 2000
#define COMMAND_PAYLOAD_SIZE     10

#define NOOP                   0x00
#define STACK_PARAM_1          5
#define STACK_PARAM_2          6
#define STACK_CHECKSUM_HB      7
#define STACK_CHECKSUM_LB      8

#define MSG_HEADER_SIZE        3
#define MSG_HEADER_HEADER      0
#define MSG_HEADER_VERSION     1
#define MSG_HEADER_MSG_LENGTH  2

#define MSG_VERSION            0xFF

#define ON                     1
#define OFF                    0

#define LO(v)                  (v & 0xFF)
#define HI(v)                  ((v >> 8) & 0xFF)
#define VAL(h, l)              ((h << 8) | l)


#define CMD_PREFIX    0x7E    /* All command start with 0x7E */
#define CMD_VERSION   0xFF    /* Not sure what the options for the version are */
#define CMD_LENGTH    0x06
#define CMD_END       0xEF

/* MP3 PLAYER COMMANDS */
#define CMD_OP_TRACK_NEXT       0x01
#define CMD_OP_TRACK_PREV       0x02
#define CMD_OP_PLAY_TRACK       0x03
#define CMD_OP_VOLUME_UP
#define CMD_OP_VOLUME_DOWN
#define CMD_OP_VOLUME_SET       0x06
#define CMD_OP_EQ               0x07
#define CMD_OP_PLAYBACK_MODE    0x09
#define CMD_OP_PLAYBACK_SOURCE
#define CMD_OP_STOP             0x16
#define CMD_OP_STANDBY
#define CMD_OP_DAC              0x1A
#define CMD_OP_RESET            0x0C
#define CMD_OP_PLAY             0x0D
#define CMD_OP_PAUSE            0x0E
#define CMD_OP_PLAYBACK_FOLDER
#define CMD_OP_REPEAT           

/* PLAYBACK MEDIA CONSTANTS */
#define PLAYBACK_MODE_FLASH     1
#define PLAYBACK_MODE_SD        2

/* EQUALIZER CONSTANTS */
#define EQ_NORMAL               0
#define EQ_ROCK                 1
#define EQ_POP                  2
#define EQ_CLASSIC              3
#define EQ_COUNTRY              4
#define EQ_JAZZ                 5

/* MP3 PLAYER COMMANDS */
#define RESPONSE_ACK             0x41 // This is really just extra noise
#define RESPONSE_ERROR           0x40
#define RESPONSE_DEVICE_IN       0x3A // Byte 7 = (1) UDisk, (2) TF/CARD, (3) PC
#define RESPONSE_DEVICE_OUT      0x3B // Byte 7 = (1) UDisk, (2) TF/CARD, (3) PC
#define RESPONSE_ONLINE          0x3F // System is online
#define RESPONSE_PLAYBACK_DONE   0x3D // Byte 7 & 8 - track number

#endif