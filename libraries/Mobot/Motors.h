#ifndef __mobotadc_h
#define __mobotadc_h

#include "Arduino.h"

#define IC1_D1 8
#define IC1_IN1 23
#define IC1_IN2 24

#define IC3_D1 7
#define IC3_IN1 25
#define IC3_IN2 26

#define IC13_SLEEP 45
#define IC13_ENABLE 46

const uint8_t M_PORTS[8] = {IC1_D1, IC1_IN1, IC1_IN2, IC3_D1, IC3_IN1, IC3_IN2, IC13_SLEEP, IC13_ENABLE};

#define M_LEFT 1
#define M_RIGHT 0
#define MDIR_FORWARD false
#define MDIR_BACKWARD true



class Motors
{
    public:
        Motors(uint8_t side);
        void setDirection(bool direction);
        void setSpeed(uint8_t speed);
        void stop();
        void fullStop();
        void start();
    private:
        volatile uint8_t motors;
        volatile bool mdir;
};

extern Motors LeftMotors;
extern Motors RightMotors;
#endif //__adc_h
