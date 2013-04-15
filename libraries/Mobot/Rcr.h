#ifndef __mobotrcr_h
#define __mobotrcr_h

#include "Arduino.h"

#define RCR_CMD_CHANNEL 0x07
#define RCR_CMD_SPEED 0x08
#define RCR_CMD_POWER 0x09
#define RCR_CMD_SENSITI 0x10
#define RCR_CMD_BUFFER 0x11

class Rcr
{
    public:
        Rcr(HardwareSerial *port);
        void setSpeed(uint8_t speed);
        void setChannel(uint8_t channel);
        void setPower(uint8_t power);
        void setSens(uint8_t sens);
        void setBuffer(uint8_t size);

    private:
        uint8_t cmds[5] = {0x43, 0x78, 0x1e, 0x00, 0x00};
        char ret[2];
        HardwareSerial *serial;
        uint16_t _write(uint8_t cmd, uint8_t param);
};

extern Rcr RadioUsart;
#endif