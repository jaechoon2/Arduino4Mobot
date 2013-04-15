#ifndef __mobotsonar_h
#define __mobotsonar_h

#define SONAR_CMD 0x10
#define SONAR_CMD_MEASURE 0x02
#define SONAR_CMD_SENSITI 0x03
#define SONAR_CMD_PULSECNT 0x04
#define SONAR_CMD_RANGE 0x05
#define SONAR_CMD_THRESHOLD 0x06
#define SONAR_CMD_SLEEP 0x07

#define SONAR_READ_DISTANCE 0x20
#define SONAR_READ_SETTINGS 0x30
#define SONAR_READ_TEST 0x40

class Sonar
{
    public:
        Sonar(uint8_t address);
        uint16_t getDistance();
        void setSens(int8_t sens);
        void setRange(uint16_t sens);
        void setThreshold(uint16_t sens);
        void setPulseCount(uint8_t cnt);
    private:
        void _write(uint8_t len);
        uint8_t status;
        uint8_t buffer[4];
        uint8_t addr;
};

extern Sonar Sonar1;
extern Sonar Sonar2;
extern Sonar Sonar3;
#endif